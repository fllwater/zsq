#include "./demo/demowin.h"

int main(int argc, char** argv)
{
	QApplication app(argc, argv);
	demowin::MyMainWindow mainWindow;
	mainWindow.show();
	return app.exec();
}


#if 1

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'demowin.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_demowin__DialogOpenCamera_t {
	QByteArrayData data[5];
	char stringdata0[60];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_demowin__DialogOpenCamera_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_demowin__DialogOpenCamera_t qt_meta_stringdata_demowin__DialogOpenCamera = {
	{
		QT_MOC_LITERAL(0, 0, 25), // "demowin::DialogOpenCamera"
		QT_MOC_LITERAL(1, 26, 8), // "sendData"
		QT_MOC_LITERAL(2, 35, 0), // ""
		QT_MOC_LITERAL(3, 36, 16), // "OpenCameraParams"
		QT_MOC_LITERAL(4, 53, 6) // "params"

	},
	"demowin::DialogOpenCamera\0sendData\0\0"
	"OpenCameraParams\0params"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_demowin__DialogOpenCamera[] = {

	// content:
	7,       // revision
	0,       // classname
	0,    0, // classinfo
	1,   14, // methods
	0,    0, // properties
	0,    0, // enums/sets
	0,    0, // constructors
	0,       // flags
	1,       // signalCount

			 // signals: name, argc, parameters, tag, flags
			 1,    1,   19,    2, 0x06 /* Public */,

			 // signals: parameters
			 QMetaType::Void, 0x80000000 | 3,    4,

			 0        // eod
};

void demowin::DialogOpenCamera::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
	if (_c == QMetaObject::InvokeMetaMethod) {
		DialogOpenCamera *_t = static_cast<DialogOpenCamera *>(_o);
		Q_UNUSED(_t)
			switch (_id) {
			case 0: _t->sendData((*reinterpret_cast< OpenCameraParams(*)>(_a[1]))); break;
			default:;
			}
	}
	else if (_c == QMetaObject::IndexOfMethod) {
		int *result = reinterpret_cast<int *>(_a[0]);
		void **func = reinterpret_cast<void **>(_a[1]);
		{
			typedef void (DialogOpenCamera::*_t)(OpenCameraParams);
			if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&DialogOpenCamera::sendData)) {
				*result = 0;
				return;
			}
		}
	}
}

const QMetaObject demowin::DialogOpenCamera::staticMetaObject = {
	{ &QDialog::staticMetaObject, qt_meta_stringdata_demowin__DialogOpenCamera.data,
	qt_meta_data_demowin__DialogOpenCamera,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR }
};


const QMetaObject *demowin::DialogOpenCamera::metaObject() const
{
	return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *demowin::DialogOpenCamera::qt_metacast(const char *_clname)
{
	if (!_clname) return Q_NULLPTR;
	if (!strcmp(_clname, qt_meta_stringdata_demowin__DialogOpenCamera.stringdata0))
		return static_cast<void*>(const_cast< DialogOpenCamera*>(this));
	return QDialog::qt_metacast(_clname);
}

int demowin::DialogOpenCamera::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
	_id = QDialog::qt_metacall(_c, _id, _a);
	if (_id < 0)
		return _id;
	if (_c == QMetaObject::InvokeMetaMethod) {
		if (_id < 1)
			qt_static_metacall(this, _c, _id, _a);
		_id -= 1;
	}
	else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
		if (_id < 1)
			*reinterpret_cast<int*>(_a[0]) = -1;
		_id -= 1;
	}
	return _id;
}

// SIGNAL 0
void demowin::DialogOpenCamera::sendData(OpenCameraParams _t1)
{
	void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
	QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
struct qt_meta_stringdata_demowin__MyMainWindow_t {
	QByteArrayData data[1];
	char stringdata0[22];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_demowin__MyMainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_demowin__MyMainWindow_t qt_meta_stringdata_demowin__MyMainWindow = {
	{
		QT_MOC_LITERAL(0, 0, 21) // "demowin::MyMainWindow"

	},
	"demowin::MyMainWindow"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_demowin__MyMainWindow[] = {

	// content:
	7,       // revision
	0,       // classname
	0,    0, // classinfo
	0,    0, // methods
	0,    0, // properties
	0,    0, // enums/sets
	0,    0, // constructors
	0,       // flags
	0,       // signalCount

	0        // eod
};

void demowin::MyMainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
	Q_UNUSED(_o);
	Q_UNUSED(_id);
	Q_UNUSED(_c);
	Q_UNUSED(_a);
}

const QMetaObject demowin::MyMainWindow::staticMetaObject = {
	{ &QMainWindow::staticMetaObject, qt_meta_stringdata_demowin__MyMainWindow.data,
	qt_meta_data_demowin__MyMainWindow,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR }
};


const QMetaObject *demowin::MyMainWindow::metaObject() const
{
	return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *demowin::MyMainWindow::qt_metacast(const char *_clname)
{
	if (!_clname) return Q_NULLPTR;
	if (!strcmp(_clname, qt_meta_stringdata_demowin__MyMainWindow.stringdata0))
		return static_cast<void*>(const_cast< MyMainWindow*>(this));
	return QMainWindow::qt_metacast(_clname);
}

int demowin::MyMainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
	_id = QMainWindow::qt_metacall(_c, _id, _a);
	if (_id < 0)
		return _id;
	return _id;
}
QT_END_MOC_NAMESPACE

#endif