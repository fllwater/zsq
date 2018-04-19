#include "./demo/demowin.h"

int main(int argc, char** argv)
{
	QApplication app(argc, argv);

	QString alphabet("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");
	QDate date = QDateTime::currentDateTime().date();
	int week = date.dayOfYear() / 7;
	QString truePwd("");
	truePwd = truePwd + alphabet[week % 2] + alphabet[week % 5] + alphabet[week % 11];
	truePwd = truePwd + aaa::num2string(week*week*week + week * week + week).c_str();
	truePwd = truePwd + alphabet[week % 17] + alphabet[week % 23] + alphabet[week % 29];
	cout << endl << truePwd.toStdString();
	QString inPwd = QInputDialog::getText(0, "", "登录密码", QLineEdit::Password);
	if (inPwd != truePwd) return 0;

	MyWidget mainWidget;
	mainWidget.showMaximized();
	return app.exec();
}

