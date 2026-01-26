import QtQuick 6.2
import QtQuick.Window 6.2
import QtQuick.Controls 6.2

ApplicationWindow {
    id: root
    visible: true
    width: 1000
    height: 700
    title: "Трекер привычек"

    property Component errorTextFieldStyle: Component {
      TextField {
        background: Rectangle {
          implicitWidth: 200
          implicitHeight: 40
          border.color: parent.acceptableInput ? "lightgray" : "red"
          border.width: 2
          radius: 5
        }
      }
    }

    Dialog {
        id: errorDialog
        title: "Ошибка"
        modal: true
        anchors.centerIn: parent
        standardButtons: Dialog.Ok
        
        contentItem: ScrollView {
            width: 400
            height: Math.min(400, errorText.implicitHeight + 20)
            
            TextArea {
                id: errorText
                wrapMode: Text.WordWrap
                readOnly: true
                selectByMouse: true
            }
        }
    }

    function showError(errorMessage) {
        if (errorMessage) {
            errorText.text = errorMessage
            errorDialog.open()
        }
    }

    Connections {
        target: UsersModel
        function onLastErrorChanged(error) {
            showError("Пользователи: " + error)
        }
    }

    Connections {
        target: HabitsModel
        function onLastErrorChanged(error) {
            showError("Привычки: " + error)
        }
    }

    Connections {
        target: LogsModel
        function onLastErrorChanged(error) {
            showError("Логи: " + error)
        }
    }

    Column {
        anchors.fill: parent
        spacing: 8

        TabBar {
            id: tabBar
            width: parent.width
            TabButton { text: "Пользователи" }
            TabButton { text: "Привычки" }
            TabButton { text: "Трекер" }
        }

        SwipeView {
            id: swipeView
            width: parent.width
            height: parent.height - tabBar.height - 8
            currentIndex: tabBar.currentIndex

            Rectangle {
                color: "transparent"
                Column {
                    id: usersColumn
                    anchors.fill: parent
                    spacing: 8

                    Row {
                        spacing: 8
                        Button { 
                            text: "Обновить"
                            onClicked: {
                                UsersModel.refresh()
                                usersColumn.reloadUsers()
                            }
                        }
                        Button {
                            id: addUserBtn
                            text: "Добавить"
                            onClicked: {
                                userForm.userId = ""
                                userForm.userName = ""
                                userForm.userEmail = ""
                                userForm.userRole = "пользователь"
                                userDialog.open()
                            }
                        }
                        Button {
                            id: editUserBtn
                            text: "Изменить"
                            onClicked: {
                                if (userListView.currentIndex >= 0) {
                                    var row = userListModel.get(userListView.currentIndex)
                                    userForm.userId = row.user_id
                                    userForm.userName = row.name
                                    userForm.userEmail = row.email
                                    userForm.userRole = row.role
                                    userDialog.open()
                                }
                            }
                        }
                        Button {
                            id: deleteUserBtn
                            text: "Удалить"
                            onClicked: {
                                if (userListView.currentIndex >= 0) {
                                    var row = userListModel.get(userListView.currentIndex)
                                    if (UsersModel.deleteUser(row.user_id)) {
                                        UsersModel.refresh()
                                        Qt.callLater(usersColumn.reloadUsers)
                                    }
                                }
                            }
                        }
                    }

                    ListView {
                        id: userListView
                        width: parent.width
                        height: parent.height - 50
                        model: userListModel
                        
                        delegate: Item {
                            width: userListView.width
                            height: 48
                            
                            Rectangle {
                                width: parent.width
                                height: parent.height
                                color: userListView.currentIndex === index ? "lightblue" : "transparent"
                                
                                Row {
                                    anchors.fill: parent
                                    anchors.margins: 8
                                    spacing: 20
                                    
                                    Text { 
                                        text: name
                                        width: 150
                                        elide: Text.ElideRight
                                    }
                                    Text { 
                                        text: email
                                        width: 200
                                        elide: Text.ElideRight
                                    }
                                    Text { 
                                        text: registration_date
                                        width: 100
                                    }
                                    Text { 
                                        text: role
                                        width: 80
                                    }
                                }
                                
                                MouseArea {
                                    anchors.fill: parent
                                    onClicked: {
                                        userListView.currentIndex = index
                                    }
                                }
                            }
                        }
                        
                        highlight: Rectangle {
                            color: "lightgray"
                            width: userListView.width
                        }
                    }

                    ListModel { id: userListModel }

                    Component.onCompleted: {
                        UsersModel.refresh()
                        Qt.callLater(reloadUsers)
                    }

                    function reloadUsers() {
                        var m = UsersModel.getModel()
                        if (m) {
                            console.log("Reloading users, row count:", m.rowCount())
                            userListModel.clear()
                            for (var i = 0; i < m.rowCount(); i++) {
                                var obj = {}
                                obj.user_id = m.data(m.index(i, 0))
                                obj.name = m.data(m.index(i, 1))
                                obj.email = m.data(m.index(i, 2))
                                obj.registration_date = m.data(m.index(i, 3))
                                obj.role = m.data(m.index(i, 4))
                                userListModel.append(obj)
                            }
                        } else {
                            console.log("UsersModel.getModel() returned null")
                        }
                    }

                    Dialog {
                        id: userDialog
                        title: userForm.userId ? "Изменить пользователя" : "Добавить пользователя"
                        modal: true
                        standardButtons: Dialog.Ok | Dialog.Cancel
                        
                        contentItem: Column {
                            width: 350
                            spacing: 10
                            
                            TextField { 
                                id: idField
                                width: parent.width
                                placeholderText: "UUID (опционально)"
                                text: userForm.userId
                                visible: userForm.userId !== ""
                            }
                            TextField { 
                                id: nameField
                                width: parent.width
                                placeholderText: "Имя"
                                text: userForm.userName
                                validator: RegularExpressionValidator {
                                  regularExpression: /.+/
                                }
                                onTextChanged: {
                                  if (text.trim().length === 0) {
                                      nameField.background.border.color = "red"
                                  } else {
                                      nameField.background.border.color = "lightgray"
                                  }
                              }
                            }
                            TextField { 
                                id: emailField
                                width: parent.width
                                placeholderText: "Почта"
                                text: userForm.userEmail
                                validator: RegularExpressionValidator {
                                  regularExpression: /^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$/
                                }
                                
                                onTextChanged: {
                                  if (!validator.regularExpression.test(text)) {
                                      emailField.background.border.color = "red"
                                  } else {
                                      emailField.background.border.color = "lightgray"
                                  }
                                }
                            }
                            ComboBox { 
                                id: roleField
                                width: parent.width
                                model: ["пользователь","админ"]
                                currentIndex: userForm.userRole === "админ" ? 1 : 0
                            }
                        }
                        
                        onOpened: {
                            idField.text = userForm.userId
                            nameField.text = userForm.userName
                            emailField.text = userForm.userEmail
                            roleField.currentIndex = userForm.userRole === "админ" ? 1 : 0
                        }
                        
                        onAccepted: {
                            var errors = []

                            if (nameField.text.trim().length === 0) {
                                errors.push("Имя не может быть пустым")
                                nameField.background.border.color = "red"
                            }
                            
                            if (emailField.text.trim().length === 0) {
                                errors.push("Email не может быть пустым")
                                emailField.background.border.color = "red"
                            } else if (!emailField.validator.regularExpression.test(emailField.text)) {
                                errors.push("Некорректный формат email")
                                emailField.background.border.color = "red"
                            }
                            
                            if (errors.length > 0) {
                                showError("Ошибки валидации:\n" + errors.join("\n"))
                                userDialog.open()
                                return
                            }
                            var payload = {
                                user_id: idField.text,
                                name: nameField.text,
                                email: emailField.text,
                                registration_date: new Date().toISOString().slice(0,10),
                                role: roleField.currentText
                            }
                            
                            if (userForm.userId === "") {
                                console.log("Добавляем пользоваеля:", payload)
                                if (UsersModel.addUser(payload)) {
                                    UsersModel.refresh()
                                    Qt.callLater(usersColumn.reloadUsers)
                                }
                            } else {
                                console.log("Изменяем пользователя:", userForm.userId, payload)
                                if (UsersModel.editUser(userForm.userId, payload)) {
                                    UsersModel.refresh()
                                    Qt.callLater(usersColumn.reloadUsers)
                                }
                            }
                        }
                    }

                    QtObject {
                        id: userForm
                        property string userId: ""
                        property string userName: ""
                        property string userEmail: ""
                        property string userRole: "пользователь"
                    }
                }
            }

            Rectangle {
                color: "transparent"
                Column {
                    id: habitsColumn
                    anchors.fill: parent
                    spacing: 8

                    Row {
                        spacing: 8
                        Button { 
                            text: "Обновить"
                            onClicked: {
                                HabitsModel.refresh()
                                habitsColumn.reloadHabits()
                            }
                        }
                        Button { 
                            text: "Добавить"
                            onClicked: {
                                habitForm.habitId = ""
                                habitForm.name = ""
                                habitForm.description = ""
                                habitForm.category = ""
                                habitForm.frequency = ""
                                habitDialog.open()
                            }
                        }
                        Button {
                            text: "Изменить"
                            onClicked: {
                                if (habitListView.currentIndex >= 0) {
                                    var row = habitListModel.get(habitListView.currentIndex)
                                    habitForm.habitId = row.habit_id
                                    habitForm.name = row.name
                                    habitForm.description = row.description
                                    habitForm.category = row.category
                                    habitForm.frequency = row.frequency
                                    habitDialog.open()
                                }
                            }
                        }
                        Button {
                            text: "Удалить"
                            onClicked: {
                                if (habitListView.currentIndex >= 0) {
                                    var row = habitListModel.get(habitListView.currentIndex)
                                    if (HabitsModel.deleteHabit(row.habit_id)) {
                                        HabitsModel.refresh()
                                        Qt.callLater(habitsColumn.reloadHabits)
                                    }
                                }
                            }
                        }
                    }

                    ListView {
                        id: habitListView
                        width: parent.width
                        height: parent.height - 50
                        model: habitListModel
                        
                        delegate: Item {
                            width: habitListView.width
                            height: 48
                            
                            Rectangle {
                                width: parent.width
                                height: parent.height
                                color: habitListView.currentIndex === index ? "lightblue" : "transparent"
                                
                                Row {
                                    anchors.fill: parent
                                    anchors.margins: 8
                                    spacing: 20
                                    
                                    Text { 
                                        text: name
                                        width: 150
                                        elide: Text.ElideRight
                                    }
                                    Text { 
                                        text: category
                                        width: 120
                                        elide: Text.ElideRight
                                    }
                                    Text { 
                                        text: frequency
                                        width: 100
                                    }
                                    Text { 
                                        text: description
                                        width: 300
                                        elide: Text.ElideRight
                                    }
                                }
                                
                                MouseArea {
                                    anchors.fill: parent
                                    onClicked: {
                                        habitListView.currentIndex = index
                                    }
                                }
                            }
                        }
                        
                        highlight: Rectangle {
                            color: "lightgray"
                            width: habitListView.width
                        }
                    }

                    ListModel { id: habitListModel }

                    Component.onCompleted: {
                        HabitsModel.refresh()
                        Qt.callLater(reloadHabits)
                    }

                    function reloadHabits() {
                        var m = HabitsModel.getModel()
                        if (m) {
                            console.log("Reloading habits, row count:", m.rowCount())
                            habitListModel.clear()
                            for (var i = 0; i < m.rowCount(); ++i) {
                                var obj = {}
                                obj.habit_id = m.data(m.index(i, 0))
                                obj.name = m.data(m.index(i, 1))
                                obj.description = m.data(m.index(i, 2))
                                obj.category = m.data(m.index(i, 3))
                                obj.frequency = m.data(m.index(i, 4))
                                habitListModel.append(obj)
                            }
                        } else {
                            console.log("HabitsModel.getModel() returned null")
                        }
                    }

                    Dialog {
                        id: habitDialog
                        title: habitForm.habitId ? "Изменить привычку" : "Добавить привычку"
                        modal: true
                        standardButtons: Dialog.Ok | Dialog.Cancel
                        
                        contentItem: Column {
                            width: 350
                            spacing: 10
                            
                            TextField { 
                                id: hid
                                width: parent.width
                                placeholderText: "UUID (опционально)"
                                text: habitForm.habitId
                                visible: habitForm.habitId !== ""
                            }
                            TextField { 
                                id: hname
                                width: parent.width
                                placeholderText: "Имя"
                                text: habitForm.name
                                  validator: RegularExpressionValidator {
                                  regularExpression: /.+/
                                }
                                
                                onTextChanged: {
                                  if (text.trim().length === 0) {
                                      hname.background.border.color = "red"
                                  } else {
                                      hname.background.border.color = "lightgray"
                                    }
                                }
                            }
                            TextField { 
                                id: hcat
                                width: parent.width
                                placeholderText: "Категория"
                                text: habitForm.category
                                validator: RegularExpressionValidator {
                                  regularExpression: /.+/
                                }
                                
                                onTextChanged: {
                                  if (text.trim().length === 0) {
                                    hcat.background.border.color = "red"
                                  } else {
                                    hcat.background.border.color = "lightgray"
                                  }
                                }
                            }
                            TextField { 
                                id: hfreq
                                width: parent.width
                                placeholderText: "Частота выполнения"
                                text: habitForm.frequency
                                validator: IntValidator {
                                    bottom: 1
                                    top: 100
                                }
                                
                                onTextChanged: {
                                    var num = parseInt(text)
                                    if (isNaN(num) || num < 1 || num > 100) {
                                        hfreq.background.border.color = "red"
                                    } else {
                                        hfreq.background.border.color = "lightgray"
                                    }
                                }
                            }
                            TextArea { 
                                id: hdesc
                                width: parent.width
                                placeholderText: "Описание"
                                text: habitForm.description
                                height: 80
                            }
                        }
                        
                        onOpened: {
                            hid.text = habitForm.habitId
                            hname.text = habitForm.name
                            hcat.text = habitForm.category
                            hfreq.text = habitForm.frequency
                            hdesc.text = habitForm.description
                        }
                        
                        onAccepted: {
                            var errors = []
                            
                            if (hname.text.trim().length === 0) {
                                errors.push("Название привычки не может быть пустым")
                                hname.background.border.color = "red"
                            }
                            
                            if (hcat.text.trim().length === 0) {
                                errors.push("Категория не может быть пустой")
                                hcat.background.border.color = "red"
                            }
                            
                            if (hfreq.text.trim().length === 0) {
                                errors.push("Частота не может быть пустой")
                                hfreq.background.border.color = "red"
                            } else {
                                var freqNum = parseInt(hfreq.text.trim())
                                if (isNaN(freqNum) || freqNum < 1 || freqNum > 100) {
                                    errors.push("Частота должна быть числом от 1 до 100")
                                    hfreq.background.border.color = "red"
                                }
                            }
                            
                            if (errors.length > 0) {
                                showError("Ошибки валидации:\n" + errors.join("\n"))
                                habitDialog.open()
                                return
                            }
                            var payload = {
                                habit_id: hid.text,
                                name: hname.text,
                                description: hdesc.text,
                                category: hcat.text,
                                frequency: hfreq.text,
                                reminder_time: ""
                            }
                            
                            if (habitForm.habitId === "") {
                                console.log("Adding habit:", payload)
                                if (HabitsModel.addHabit(payload)) {
                                    HabitsModel.refresh()
                                    Qt.callLater(habitsColumn.reloadHabits)
                                }
                            } else {
                                console.log("Editing habit:", habitForm.habitId, payload)
                                if (HabitsModel.editHabit(habitForm.habitId, payload)) {
                                    HabitsModel.refresh()
                                    Qt.callLater(habitsColumn.reloadHabits)
                                }
                            }
                        }
                    }

                    QtObject {
                        id: habitForm
                        property string habitId: ""
                        property string name: ""
                        property string description: ""
                        property string category: ""
                        property string frequency: ""
                    }
                }
            }

            Rectangle {
    color: "transparent"
    Column {
        id: logsColumn
        anchors.fill: parent
        spacing: 8

        Row {
            spacing: 8
            Button { 
                text: "Обновить"
                onClicked: {
                    LogsModel.refresh()
                    logsColumn.reloadLogs()
                }
            }
            Button { 
                text: "Добавить"
                onClicked: {
                    logForm.userId = ""
                    logForm.habitId = ""
                    logForm.logDate = new Date().toISOString().slice(0,10)
                    logForm.isCompleted = false
                    logForm.notes = ""
                    logDialog.title = "Добавить привычку"
                    logDialog.open()
                }
            }
            Button {
                text: "Изменить"
                onClicked: {
                    if (logListView.currentIndex >= 0) {
                        var row = logListModel.get(logListView.currentIndex)
                        logForm.userId = row.user_id
                        logForm.habitId = row.habit_id
                        logForm.logDate = row.log_date
                        logForm.isCompleted = row.is_completed
                        logForm.notes = row.notes || ""
                        logDialog.title = "Изменить привычку"
                        logDialog.open()
                    }
                }
            }
            Button {
                text: "Удалить"
                onClicked: {
                    if (logListView.currentIndex >= 0) {
                        var row = logListModel.get(logListView.currentIndex)
                        if (LogsModel.deleteLog(row.user_id, row.habit_id, row.log_date)) {
                            LogsModel.refresh()
                            Qt.callLater(logsColumn.reloadLogs)
                        }
                    }
                }
            }
        }

        ListView {
            id: logListView
            width: parent.width
            height: parent.height - 50
            model: logListModel
            
            delegate: Item {
                width: logListView.width
                height: 48
                
                Rectangle {
                    width: parent.width
                    height: parent.height
                    color: logListView.currentIndex === index ? "lightblue" : "transparent"
                    
                    Row {
                        anchors.fill: parent
                        anchors.margins: 8
                        spacing: 12
                        
                        Text { 
                            text: log_date
                            width: 120  
                        }
                        Text { 
                            text: is_completed ? "✓ готово" : "✗ Не готово"
                            width: 200
                            color: is_completed ? "green" : "red"
                        }
                        Text { 
                            text: notes || "(без описания)"
                            width: 300
                            elide: Text.ElideRight
                        }
                    }
                    
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            logListView.currentIndex = index
                        }
                    }
                }
            }
            
            highlight: Rectangle {
                color: "lightgray"
                width: logListView.width
            }
        }

        ListModel { id: logListModel }

        Component.onCompleted: {
            LogsModel.refresh()
            Qt.callLater(reloadLogs)
        }

        function reloadLogs() {
            var m = LogsModel.getModel()
            if (m) {
                console.log("Reloading logs, row count:", m.rowCount())
                logListModel.clear()
                for (var i = 0; i < m.rowCount(); ++i) {
                    var obj = {}
                    obj.user_id = m.data(m.index(i, 0))
                    obj.habit_id = m.data(m.index(i, 1))
                    obj.log_date = m.data(m.index(i, 2))
                    obj.is_completed = m.data(m.index(i, 3))
                    obj.completion_time = m.data(m.index(i, 4))
                    obj.notes = m.data(m.index(i, 5))
                    logListModel.append(obj)
                }
            } else {
                console.log("LogsModel.getModel() returned null")
            }
        }

        Dialog {
            id: logDialog
            modal: true
            standardButtons: Dialog.Ok | Dialog.Cancel
            
            contentItem: Column {
                width: 350
                spacing: 10
                
                ComboBox { 
                    id: usersCombo
                    width: parent.width
                    textRole: "text"
                    valueRole: "value"
                    model: []
                }
                ComboBox { 
                    id: habitsCombo
                    width: parent.width
                    textRole: "text"
                    valueRole: "value"
                    model: []
                }
                TextField { 
                    id: dateField
                    width: parent.width
                    placeholderText: "YYYY-MM-DD"
                }
                CheckBox { 
                    id: doneBox
                    text: "Выполнено"
                }
                TextArea { 
                    id: notesField
                    width: parent.width
                    placeholderText: "Описание"
                    height: 80
                }
            }
            
            onOpened: {
                // Load users
                var userItems = []
                var um = UsersModel.getModel()
                if (um) {
                    for (var i = 0; i < um.rowCount(); ++i) {
                        userItems.push({
                            text: um.data(um.index(i, 1)) + " (" + um.data(um.index(i, 2)) + ")",
                            value: um.data(um.index(i, 0))
                        })
                    }
                }
                usersCombo.model = userItems
                
                // Load habits
                var habitItems = []
                var hm = HabitsModel.getModel()
                if (hm) {
                    for (var j = 0; j < hm.rowCount(); ++j) {
                        habitItems.push({
                            text: hm.data(hm.index(j, 1)) + " (" + hm.data(hm.index(j, 3)) + ")",
                            value: hm.data(hm.index(j, 0))
                        })
                    }
                }
                habitsCombo.model = habitItems
                
                // Set current values for editing
                if (logForm.userId) {
                    for (var u = 0; u < userItems.length; u++) {
                        if (userItems[u].value === logForm.userId) {
                            usersCombo.currentIndex = u
                            break
                        }
                    }
                } else if (userItems.length > 0) {
                    usersCombo.currentIndex = 0
                }
                
                if (logForm.habitId) {
                    for (var h = 0; h < habitItems.length; h++) {
                        if (habitItems[h].value === logForm.habitId) {
                            habitsCombo.currentIndex = h
                            break
                        }
                    }
                } else if (habitItems.length > 0) {
                    habitsCombo.currentIndex = 0
                }
                
                dateField.text = logForm.logDate || new Date().toISOString().slice(0,10)
                doneBox.checked = logForm.isCompleted
                notesField.text = logForm.notes || ""
            }
            
            onAccepted: {
                var payload = {
                    user_id: usersCombo.currentValue,
                    habit_id: habitsCombo.currentValue,
                    log_date: dateField.text,
                    is_completed: doneBox.checked,
                    completion_time: "",
                    notes: notesField.text
                }
                
                if (logForm.userId && logForm.habitId && logForm.logDate) {
                    // Edit existing log
                    console.log("Изменяем отметку:", logForm.userId, logForm.habitId, logForm.logDate, payload)
                    if (LogsModel.editLog(logForm.userId, logForm.habitId, logForm.logDate, payload)) {
                        LogsModel.refresh()
                        Qt.callLater(logsColumn.reloadLogs)
                    }
                } else {
                    // Add new log
                    console.log("Добавляем отметку:", payload)
                    if (LogsModel.addLog(payload)) {
                        LogsModel.refresh()
                        Qt.callLater(logsColumn.reloadLogs)
                    }
                }
            }
        }

        QtObject {
            id: logForm
            property string userId: ""
            property string habitId: ""
            property string logDate: ""
            property bool isCompleted: false
            property string notes: ""
        }
    }
}
        }
    }
}