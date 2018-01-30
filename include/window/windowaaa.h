#include "base/azz.h"

#ifndef __windowaaa_h__
#define __windowaaa_h__

namespace windowaaa
{
	void checkScreenInfo(QApplication *app)
	{
		//1.
		cout << endl << endl << "With QDesktopWidget";
		QDesktopWidget desk;
		cout << endl << "\t" << "screenCount = " << desk.screenCount();
		for (int i = 0; i < desk.screenCount(); ++i)
		{
			QWidget *screen = desk.screen(i);

			cout << endl << "\t" << i << " screen logicalDpiX = " << screen->logicalDpiX();
			cout << endl << "\t" << i << " screen logicalDpiY = " << screen->logicalDpiY();
			cout << endl << "\t" << i << " screen physicalDpiX = " << screen->physicalDpiX();
			cout << endl << "\t" << i << " screen physicalDpiY = " << screen->physicalDpiY();
			cout << endl << "\t" << i << " screen physicalDpiY = " << screen->physicalDpiY();
			cout << endl << "\t" << i << " screen width = " << screen->width();
			cout << endl << "\t" << i << " screen height = " << screen->height();
			cout << endl << "\t" << i << " screen widthMM = " << screen->widthMM();
			cout << endl << "\t" << i << " screen heightMM = " << screen->heightMM();
		}

		//2.
		cout << endl << endl << "With QScreen";
		QScreen *screen = app->primaryScreen();
		cout << endl << "\t" << "logicalDotsPerInchX = " << screen->logicalDotsPerInchX();
		cout << endl << "\t" << "logicalDotsPerInchY = " << screen->logicalDotsPerInchY();
		cout << endl << "\t" << "logicalDotsPerInch = " << screen->logicalDotsPerInch();
		cout << endl << "\t" << "physicalDotsPerInchX = " << screen->physicalDotsPerInchX();
		cout << endl << "\t" << "physicalDotsPerInchY = " << screen->physicalDotsPerInchY();
		cout << endl << "\t" << "physicalDotsPerInch = " << screen->physicalDotsPerInch();
		cout << endl << "\t" << "size = " << screen->size().width() << " x " << screen->size().height();
		cout << endl << "\t" << "physicalSize = " << screen->physicalSize().width() << " x " << screen->physicalSize().height();

		//3.
#ifdef WIN32
		cout << endl << endl << "With WinAPI";
		cout << endl << "\t" << "GetDeviceCaps(GetDC(NULL), LOGPIXELSX) = " << GetDeviceCaps(GetDC(NULL), LOGPIXELSX);
		cout << endl << "\t" << "GetDeviceCaps(GetDC(NULL), LOGPIXELSY) = " << GetDeviceCaps(GetDC(NULL), LOGPIXELSY);
		cout << endl << "\t" << "GetDeviceCaps(GetDC(NULL), PHYSICALWIDTH) = " << GetDeviceCaps(GetDC(NULL), PHYSICALWIDTH);
		cout << endl << "\t" << "GetDeviceCaps(GetDC(NULL), PHYSICALHEIGHT) = " << GetDeviceCaps(GetDC(NULL), PHYSICALHEIGHT);
		cout << endl << "\t" << "GetDeviceCaps(GetDC(NULL), HORZRES) = " << GetDeviceCaps(GetDC(NULL), HORZRES);
		cout << endl << "\t" << "GetDeviceCaps(GetDC(NULL), VERTRES) = " << GetDeviceCaps(GetDC(NULL), VERTRES);
		cout << endl << "\t" << "GetDeviceCaps(GetDC(NULL), HORZSIZE) = " << GetDeviceCaps(GetDC(NULL), HORZSIZE);
		cout << endl << "\t" << "GetDeviceCaps(GetDC(NULL), VERTSIZE) = " << GetDeviceCaps(GetDC(NULL), VERTSIZE);
		cout << endl << "\t" << "GetSystemMetrics(SM_CXSCREEN) = " << GetSystemMetrics(SM_CXSCREEN);
		cout << endl << "\t" << "GetSystemMetrics(SM_CYSCREEN) = " << GetSystemMetrics(SM_CYSCREEN);
#endif
	}
}
#endif
