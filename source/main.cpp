#include "./demo/demowin.h"

int main(int argc, char** argv)
{
	QApplication app(argc, argv);
	MyWidget mainWidget;
	mainWidget.show();
	return app.exec();
}
