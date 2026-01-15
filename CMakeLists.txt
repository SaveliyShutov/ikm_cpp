cmake_minimum_required(VERSION 3.16)
project(habit_tracker_qt LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Quick QuickControls2 Sql Qml)

file(GLOB_RECURSE SRC_FILES
    src/*.cpp
    src/*.h
)

add_executable(habit-tracker
    ${SRC_FILES}
    qml/main.qml
)

qt_add_qml_module(habit-tracker
    URI HabitTracker
    VERSION 1.0
    QML_FILES qml/main.qml
)

target_include_directories(habit-tracker PRIVATE src)
target_link_libraries(habit-tracker PRIVATE Qt6::Core Qt6::Quick Qt6::QuickControls2 Qt6::Sql Qt6::Qml)
