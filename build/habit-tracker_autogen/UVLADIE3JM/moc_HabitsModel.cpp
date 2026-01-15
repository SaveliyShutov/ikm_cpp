/****************************************************************************
** Meta object code from reading C++ file 'HabitsModel.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../src/HabitsModel.h"
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'HabitsModel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.4.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
namespace {
struct qt_meta_stringdata_HabitsModel_t {
    uint offsetsAndSizes[18];
    char stringdata0[12];
    char stringdata1[9];
    char stringdata2[1];
    char stringdata3[8];
    char stringdata4[9];
    char stringdata5[5];
    char stringdata6[10];
    char stringdata7[3];
    char stringdata8[12];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_HabitsModel_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_HabitsModel_t qt_meta_stringdata_HabitsModel = {
    {
        QT_MOC_LITERAL(0, 11),  // "HabitsModel"
        QT_MOC_LITERAL(12, 8),  // "getModel"
        QT_MOC_LITERAL(21, 0),  // ""
        QT_MOC_LITERAL(22, 7),  // "refresh"
        QT_MOC_LITERAL(30, 8),  // "addHabit"
        QT_MOC_LITERAL(39, 4),  // "data"
        QT_MOC_LITERAL(44, 9),  // "editHabit"
        QT_MOC_LITERAL(54, 2),  // "id"
        QT_MOC_LITERAL(57, 11)   // "deleteHabit"
    },
    "HabitsModel",
    "getModel",
    "",
    "refresh",
    "addHabit",
    "data",
    "editHabit",
    "id",
    "deleteHabit"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_HabitsModel[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // methods: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   44,    2, 0x02,    1 /* Public */,
       3,    0,   45,    2, 0x02,    2 /* Public */,
       4,    1,   46,    2, 0x02,    3 /* Public */,
       6,    2,   49,    2, 0x02,    5 /* Public */,
       8,    1,   54,    2, 0x02,    8 /* Public */,

 // methods: parameters
    QMetaType::QVariant,
    QMetaType::Void,
    QMetaType::Bool, QMetaType::QVariantMap,    5,
    QMetaType::Bool, QMetaType::QString, QMetaType::QVariantMap,    7,    5,
    QMetaType::Bool, QMetaType::QString,    7,

       0        // eod
};

Q_CONSTINIT const QMetaObject HabitsModel::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_HabitsModel.offsetsAndSizes,
    qt_meta_data_HabitsModel,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_HabitsModel_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<HabitsModel, std::true_type>,
        // method 'getModel'
        QtPrivate::TypeAndForceComplete<QVariant, std::false_type>,
        // method 'refresh'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'addHabit'
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QVariantMap &, std::false_type>,
        // method 'editHabit'
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QVariantMap &, std::false_type>,
        // method 'deleteHabit'
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>
    >,
    nullptr
} };

void HabitsModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<HabitsModel *>(_o);
        (void)_t;
        switch (_id) {
        case 0: { QVariant _r = _t->getModel();
            if (_a[0]) *reinterpret_cast< QVariant*>(_a[0]) = std::move(_r); }  break;
        case 1: _t->refresh(); break;
        case 2: { bool _r = _t->addHabit((*reinterpret_cast< std::add_pointer_t<QVariantMap>>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 3: { bool _r = _t->editHabit((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QVariantMap>>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 4: { bool _r = _t->deleteHabit((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    }
}

const QMetaObject *HabitsModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *HabitsModel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_HabitsModel.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int HabitsModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 5;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
