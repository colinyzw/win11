/****************************************************************************
** Meta object code from reading C++ file 'logindialog.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../MyChat/logindialog.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'logindialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_LoginDialog_t {
    QByteArrayData data[12];
    char stringdata0[181];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_LoginDialog_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_LoginDialog_t qt_meta_stringdata_LoginDialog = {
    {
QT_MOC_LITERAL(0, 0, 11), // "LoginDialog"
QT_MOC_LITERAL(1, 12, 15), // "SIG_LoginCommit"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 3), // "tel"
QT_MOC_LITERAL(4, 33, 8), // "password"
QT_MOC_LITERAL(5, 42, 23), // "SIG_LoginCommitRegister"
QT_MOC_LITERAL(6, 66, 4), // "name"
QT_MOC_LITERAL(7, 71, 9), // "SIG_Close"
QT_MOC_LITERAL(8, 81, 19), // "on_pb_clear_clicked"
QT_MOC_LITERAL(9, 101, 20), // "on_pb_commit_clicked"
QT_MOC_LITERAL(10, 122, 28), // "on_pb_clear_register_clicked"
QT_MOC_LITERAL(11, 151, 29) // "on_pb_commit_register_clicked"

    },
    "LoginDialog\0SIG_LoginCommit\0\0tel\0"
    "password\0SIG_LoginCommitRegister\0name\0"
    "SIG_Close\0on_pb_clear_clicked\0"
    "on_pb_commit_clicked\0on_pb_clear_register_clicked\0"
    "on_pb_commit_register_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_LoginDialog[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   49,    2, 0x06 /* Public */,
       5,    3,   54,    2, 0x06 /* Public */,
       7,    0,   61,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    0,   62,    2, 0x08 /* Private */,
       9,    0,   63,    2, 0x08 /* Private */,
      10,    0,   64,    2, 0x08 /* Private */,
      11,    0,   65,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    3,    4,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString,    3,    4,    6,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void LoginDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        LoginDialog *_t = static_cast<LoginDialog *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->SIG_LoginCommit((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 1: _t->SIG_LoginCommitRegister((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 2: _t->SIG_Close(); break;
        case 3: _t->on_pb_clear_clicked(); break;
        case 4: _t->on_pb_commit_clicked(); break;
        case 5: _t->on_pb_clear_register_clicked(); break;
        case 6: _t->on_pb_commit_register_clicked(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (LoginDialog::*_t)(QString , QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&LoginDialog::SIG_LoginCommit)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (LoginDialog::*_t)(QString , QString , QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&LoginDialog::SIG_LoginCommitRegister)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (LoginDialog::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&LoginDialog::SIG_Close)) {
                *result = 2;
                return;
            }
        }
    }
}

const QMetaObject LoginDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_LoginDialog.data,
      qt_meta_data_LoginDialog,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *LoginDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *LoginDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_LoginDialog.stringdata0))
        return static_cast<void*>(const_cast< LoginDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int LoginDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void LoginDialog::SIG_LoginCommit(QString _t1, QString _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void LoginDialog::SIG_LoginCommitRegister(QString _t1, QString _t2, QString _t3)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void LoginDialog::SIG_Close()
{
    QMetaObject::activate(this, &staticMetaObject, 2, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
