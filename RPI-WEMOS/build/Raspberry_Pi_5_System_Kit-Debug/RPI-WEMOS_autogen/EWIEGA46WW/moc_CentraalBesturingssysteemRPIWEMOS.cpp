/****************************************************************************
** Meta object code from reading C++ file 'CentraalBesturingssysteemRPIWEMOS.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.8.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../CentraalBesturingssysteemRPIWEMOS.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CentraalBesturingssysteemRPIWEMOS.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.8.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN33CentraalBesturingssysteemRPIWEMOSE_t {};
} // unnamed namespace


#ifdef QT_MOC_HAS_STRINGDATA
static constexpr auto qt_meta_stringdata_ZN33CentraalBesturingssysteemRPIWEMOSE = QtMocHelpers::stringData(
    "CentraalBesturingssysteemRPIWEMOS",
    "stuurNetwerkCommando",
    "",
    "commando",
    "stuurBifrostBericht",
    "topic",
    "payload",
    "ventilatorStatusGewijzigd",
    "aan",
    "brandAlarmStatusGewijzigd",
    "actief",
    "overruleStatusGewijzigd",
    "logBerichtGegenereerd",
    "bericht",
    "tafelStatusGewijzigd",
    "id",
    "hulpNodig",
    "bewegingStatusGewijzigd",
    "beweging",
    "rgbStatusGewijzigd",
    "rgbWaarde",
    "activeerBrandOverrule",
    "verwerkBifrostRune",
    "resetTafel",
    "zetRgbKleur",
    "kleurNaam",
    "zetRgbUit",
    "zetRgbAutoModus",
    "zetRgbKleurKeuze"
);
#else  // !QT_MOC_HAS_STRINGDATA
#error "qtmochelpers.h not found or too old."
#endif // !QT_MOC_HAS_STRINGDATA

Q_CONSTINIT static const uint qt_meta_data_ZN33CentraalBesturingssysteemRPIWEMOSE[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
      16,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       9,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,  110,    2, 0x06,    1 /* Public */,
       4,    2,  113,    2, 0x06,    3 /* Public */,
       7,    1,  118,    2, 0x06,    6 /* Public */,
       9,    1,  121,    2, 0x06,    8 /* Public */,
      11,    1,  124,    2, 0x06,   10 /* Public */,
      12,    1,  127,    2, 0x06,   12 /* Public */,
      14,    2,  130,    2, 0x06,   14 /* Public */,
      17,    1,  135,    2, 0x06,   17 /* Public */,
      19,    1,  138,    2, 0x06,   19 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
      21,    0,  141,    2, 0x0a,   21 /* Public */,
      22,    2,  142,    2, 0x0a,   22 /* Public */,
      23,    1,  147,    2, 0x0a,   25 /* Public */,
      24,    1,  150,    2, 0x0a,   27 /* Public */,
      26,    0,  153,    2, 0x0a,   29 /* Public */,
      27,    1,  154,    2, 0x0a,   30 /* Public */,
      28,    1,  157,    2, 0x0a,   32 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    5,    6,
    QMetaType::Void, QMetaType::Bool,    8,
    QMetaType::Void, QMetaType::Bool,   10,
    QMetaType::Void, QMetaType::Bool,   10,
    QMetaType::Void, QMetaType::QString,   13,
    QMetaType::Void, QMetaType::Int, QMetaType::Bool,   15,   16,
    QMetaType::Void, QMetaType::Bool,   18,
    QMetaType::Void, QMetaType::QString,   20,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    5,    6,
    QMetaType::Void, QMetaType::Int,   15,
    QMetaType::Void, QMetaType::QString,   25,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    8,
    QMetaType::Void, QMetaType::QString,   25,

       0        // eod
};

Q_CONSTINIT const QMetaObject CentraalBesturingssysteemRPIWEMOS::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_ZN33CentraalBesturingssysteemRPIWEMOSE.offsetsAndSizes,
    qt_meta_data_ZN33CentraalBesturingssysteemRPIWEMOSE,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_tag_ZN33CentraalBesturingssysteemRPIWEMOSE_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<CentraalBesturingssysteemRPIWEMOS, std::true_type>,
        // method 'stuurNetwerkCommando'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'stuurBifrostBericht'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'ventilatorStatusGewijzigd'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'brandAlarmStatusGewijzigd'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'overruleStatusGewijzigd'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'logBerichtGegenereerd'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'tafelStatusGewijzigd'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'bewegingStatusGewijzigd'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'rgbStatusGewijzigd'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'activeerBrandOverrule'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'verwerkBifrostRune'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'resetTafel'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'zetRgbKleur'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'zetRgbUit'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'zetRgbAutoModus'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'zetRgbKleurKeuze'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>
    >,
    nullptr
} };

void CentraalBesturingssysteemRPIWEMOS::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<CentraalBesturingssysteemRPIWEMOS *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->stuurNetwerkCommando((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 1: _t->stuurBifrostBericht((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 2: _t->ventilatorStatusGewijzigd((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 3: _t->brandAlarmStatusGewijzigd((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 4: _t->overruleStatusGewijzigd((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 5: _t->logBerichtGegenereerd((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 6: _t->tafelStatusGewijzigd((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<bool>>(_a[2]))); break;
        case 7: _t->bewegingStatusGewijzigd((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 8: _t->rgbStatusGewijzigd((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 9: _t->activeerBrandOverrule(); break;
        case 10: _t->verwerkBifrostRune((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 11: _t->resetTafel((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 12: _t->zetRgbKleur((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 13: _t->zetRgbUit(); break;
        case 14: _t->zetRgbAutoModus((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 15: _t->zetRgbKleurKeuze((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _q_method_type = void (CentraalBesturingssysteemRPIWEMOS::*)(const QString & );
            if (_q_method_type _q_method = &CentraalBesturingssysteemRPIWEMOS::stuurNetwerkCommando; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _q_method_type = void (CentraalBesturingssysteemRPIWEMOS::*)(const QString & , const QString & );
            if (_q_method_type _q_method = &CentraalBesturingssysteemRPIWEMOS::stuurBifrostBericht; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _q_method_type = void (CentraalBesturingssysteemRPIWEMOS::*)(bool );
            if (_q_method_type _q_method = &CentraalBesturingssysteemRPIWEMOS::ventilatorStatusGewijzigd; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
        {
            using _q_method_type = void (CentraalBesturingssysteemRPIWEMOS::*)(bool );
            if (_q_method_type _q_method = &CentraalBesturingssysteemRPIWEMOS::brandAlarmStatusGewijzigd; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 3;
                return;
            }
        }
        {
            using _q_method_type = void (CentraalBesturingssysteemRPIWEMOS::*)(bool );
            if (_q_method_type _q_method = &CentraalBesturingssysteemRPIWEMOS::overruleStatusGewijzigd; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 4;
                return;
            }
        }
        {
            using _q_method_type = void (CentraalBesturingssysteemRPIWEMOS::*)(const QString & );
            if (_q_method_type _q_method = &CentraalBesturingssysteemRPIWEMOS::logBerichtGegenereerd; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 5;
                return;
            }
        }
        {
            using _q_method_type = void (CentraalBesturingssysteemRPIWEMOS::*)(int , bool );
            if (_q_method_type _q_method = &CentraalBesturingssysteemRPIWEMOS::tafelStatusGewijzigd; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 6;
                return;
            }
        }
        {
            using _q_method_type = void (CentraalBesturingssysteemRPIWEMOS::*)(bool );
            if (_q_method_type _q_method = &CentraalBesturingssysteemRPIWEMOS::bewegingStatusGewijzigd; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 7;
                return;
            }
        }
        {
            using _q_method_type = void (CentraalBesturingssysteemRPIWEMOS::*)(const QString & );
            if (_q_method_type _q_method = &CentraalBesturingssysteemRPIWEMOS::rgbStatusGewijzigd; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 8;
                return;
            }
        }
    }
}

const QMetaObject *CentraalBesturingssysteemRPIWEMOS::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CentraalBesturingssysteemRPIWEMOS::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ZN33CentraalBesturingssysteemRPIWEMOSE.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int CentraalBesturingssysteemRPIWEMOS::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 16)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 16;
    }
    return _id;
}

// SIGNAL 0
void CentraalBesturingssysteemRPIWEMOS::stuurNetwerkCommando(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CentraalBesturingssysteemRPIWEMOS::stuurBifrostBericht(const QString & _t1, const QString & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void CentraalBesturingssysteemRPIWEMOS::ventilatorStatusGewijzigd(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void CentraalBesturingssysteemRPIWEMOS::brandAlarmStatusGewijzigd(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void CentraalBesturingssysteemRPIWEMOS::overruleStatusGewijzigd(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void CentraalBesturingssysteemRPIWEMOS::logBerichtGegenereerd(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void CentraalBesturingssysteemRPIWEMOS::tafelStatusGewijzigd(int _t1, bool _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void CentraalBesturingssysteemRPIWEMOS::bewegingStatusGewijzigd(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void CentraalBesturingssysteemRPIWEMOS::rgbStatusGewijzigd(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}
QT_WARNING_POP
