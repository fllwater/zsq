﻿#include "window/windowaaa.h"
#include "InterfaceLLT_2.h"
#include "qcustomplot.h"


#ifndef __demowin_h__
#define __demowin_h__


class MyWidget : public QWidget
{
//Device control module

public://Serialport function
	queue<uchar> queueXYZR;
	void serialPortXYZR_readyRead() 
	{
		cout << endl << "1";
		//1.
		QByteArray xyz= serialPortXYZR.readAll();
		for (int i = 0; i < xyz.size(); ++i) queueXYZR.push((uchar)xyz[i]);

		//2.
		while (queueXYZR.size() >= 20 && queueXYZR.front() != 0x57) queueXYZR.pop();

		//3.
		if (queueXYZR.size() < 20) return;

		//4.
		uchar data[18], sumCalc = (uchar)0;
		queueXYZR.pop();
		for (int i = 0; i < 18; ++i)
		{
			data[i] = queueXYZR.front();
			queueXYZR.pop();
			sumCalc += data[i];
		}
		uchar sumTrue = queueXYZR.front();
		queueXYZR.pop();
		
		//5.
		if (sumCalc != sumTrue) return;

		//6.
		lineEditXAxis->setText(aaa::num2string(((int*)(data + 1))[0]).c_str());
		lineEditYAxis->setText(aaa::num2string(((int*)(data + 1))[1]).c_str());
		lineEditZAxis->setText(aaa::num2string(((int*)(data + 1))[2]).c_str());
		lineEditRotate->setText(aaa::num2string(((int*)(data + 1))[3]).c_str());
	}
	void serialPortXYZR_bytesWritten() {}
	void serialPortXYZR_error(QSerialPort::SerialPortError error) { if (error > 0) QMessageBox::warning(this, "Warning", QString("Error occured and the error code: ") + aaa::num2string(error).c_str()); }

	void serialPortCtrl_readyRead() {}
	void serialPortCtrl_bytesWritten() {}
	void serialPortCtrl_error() {}

public://Sqlite function
	long timeid = -1;
	long scanid = -1;
	bool saveScan = false;
	void GetAndSaveAndShowProfiles()
	{
		//0.Define variables
		vector<double> vdValueX(m_uiResolution);
		vector<double> vdValueZ(m_uiResolution);
		vector<unsigned char> vucProfileBuffer(m_uiResolution * 4 + 16);//Resize the profile buffer to the maximal profile size

																		//1.Get profiles
		if ((iRetValue = m_pLLT->GetActualProfile(&vucProfileBuffer[0], (uint)vucProfileBuffer.size(), PURE_PROFILE, NULL)) != vucProfileBuffer.size())
		{
			QMessageBox::warning(this, "", QString("1.Get profiles: Err \nError code: ") + aaa::num2string(iRetValue).c_str());
			return;
		}
		//else cout << endl << "1.Get profiles: OK";

		//2.Convert profiles
		iRetValue = m_pLLT->ConvertProfile2Values(&vucProfileBuffer[0], m_uiResolution, PURE_PROFILE, m_tscanCONTROLType, 0, true, NULL, NULL, NULL, &vdValueX[0], &vdValueZ[0], NULL, NULL);
		if (((iRetValue & CONVERT_X) == 0) || ((iRetValue & CONVERT_Z) == 0))
		{
			QMessageBox::warning(this, "", QString("2.Convert profiles: Err \nError code: ") + aaa::num2string(iRetValue).c_str());
			return;
		}
		//else cout << endl << "2.Convert profiles: OK";

		//3.Save profiles
		if (saveScan)
		{
			QSqlQuery query(db);
			query.prepare("insert into tb_scan_details (timeid, scanid, xdata, zdata) values (?, ?, ?, ?)");
			query.addBindValue(timeid);
			query.addBindValue(scanid++);
			query.addBindValue(QByteArray::fromRawData((char*)(&vdValueX[0]), (int)vdValueX.size() * sizeof(double)));
			query.addBindValue(QByteArray::fromRawData((char*)(&vdValueZ[0]), (int)vdValueZ.size() * sizeof(double)));
			query.exec();
			saveScan = false;
			static int n = 0;
			cout << endl << "save: " << ++n;
		}

		//4.Show profiles
		vector<double>::iterator minX = min_element(std::begin(vdValueX), std::end(vdValueX));
		vector<double>::iterator maxX = max_element(std::begin(vdValueX), std::end(vdValueX));
		vector<double>::iterator minZ = min_element(std::begin(vdValueZ), std::end(vdValueZ));
		vector<double>::iterator maxZ = max_element(std::begin(vdValueZ), std::end(vdValueZ));
		QVector<double> vdValueXX = QVector<double>::fromStdVector(vdValueX);
		QVector<double> vdValueZZ = QVector<double>::fromStdVector(vdValueZ);
		customPlotViewScan->xAxis->setRange(*minX, *maxX);
		customPlotViewScan->yAxis->setRange(*minZ, *maxZ);
		customPlotViewScan->graph(0)->setData(vdValueXX, vdValueZZ);
		customPlotViewScan->replot();
	}
	void EnableOrDisableControls(bool enable)
	{
		pushButtonMoveRight->setEnabled(enable);
		pushButtonMoveLeft->setEnabled(enable);
		pushButtonMoveForward->setEnabled(enable);
		pushButtonMoveBackward->setEnabled(enable);
		pushButtonMoveUp->setEnabled(enable);
		pushButtonMoveDown->setEnabled(enable);
		pushButtonRotateClockwise->setEnabled(enable);
		pushButtonRotateAntiClockwise->setEnabled(enable);
		pushButtonReset->setEnabled(enable);
		PushButtonStartup->setText(enable ? "Startup" : "Stop");
	}

public://Ethernet function
	CInterfaceLLT* m_pLLT = NULL;
	TScannerType m_tscanCONTROLType;
	uint m_uiResolution = 0;
	int iRetValue = 0;//buffer
	bool exitDevice() {}
	bool initDevice(uint uiShutterTime = 100, uint uiIdleTime = 900)
	{
#if 1
		//1.Load LLT.dll
		bool bLoadError;
		m_pLLT = new CInterfaceLLT("LLT.dll", &bLoadError);
		if (bLoadError)
		{
			QMessageBox::warning(this, "", "1.Load LLT.dll: Err");
			delete m_pLLT;
			return false;
		}
		else cout << endl << "1.Load LLT.dll: OK";

		//2.Check LLT.dll's feasibility
		if (m_pLLT->m_pFunctions->CreateLLTDevice == NULL)
		{
			QMessageBox::warning(this, "", "2.Check LLT.dll's feasibility: Err");
			delete m_pLLT;
			return false;
		}
		else cout << endl << "2.Check LLT.dll's feasibility: OK";

		//3.Create firewire device
		if (!m_pLLT->CreateLLTDevice(INTF_TYPE_ETHERNET))
		{
			QMessageBox::warning(this, "", "3.Create firewire device: Err");
			delete m_pLLT;
			return false;
		}
		else cout << endl << "3.Create firewire device: OK";
#endif

#if 1
		//4.Get available interfaces
		uint vuiEthernetInterfaces[50];
		if ((iRetValue = m_pLLT->GetDeviceInterfacesFast(vuiEthernetInterfaces, 50)) < GENERAL_FUNCTION_OK)
		{
			QMessageBox::warning(this, "", "4.Get available interfaces: Err");
			delete m_pLLT;
			return false;
		}
		else cout << endl << "4.Get available interfaces: OK and " << iRetValue << " interfaces";

		//5.Set ethernet interface
		if ((iRetValue = m_pLLT->SetDeviceInterface(vuiEthernetInterfaces[0], 0)) < GENERAL_FUNCTION_OK)
		{
			QMessageBox::warning(this, "", QString("5.Set ethernet interface: Err \nError code: ") + aaa::num2string(iRetValue).c_str());
			delete m_pLLT;
			return false;
		}
		else cout << endl << "5.Set ethernet interface: OK";

		//6.Connect to scanCONTROL-device
		if ((iRetValue = m_pLLT->Connect()) < GENERAL_FUNCTION_OK)
		{
			QMessageBox::warning(this, "", QString("6.Connect to scanCONTROL-device: Err \nError code: ") + aaa::num2string(iRetValue).c_str());
			delete m_pLLT;
			return false;
		}
		else cout << endl << "6.Connect to scanCONTROL-device: OK";

		//7.Get scanCONTROL type
		if ((iRetValue = m_pLLT->GetLLTType(&m_tscanCONTROLType)) < GENERAL_FUNCTION_OK || iRetValue == GENERAL_FUNCTION_DEVICE_NAME_NOT_SUPPORTED)
		{
			QMessageBox::warning(this, "", QString("7.Get scanCONTROL type: Err \nError code: ") + aaa::num2string(iRetValue).c_str() + " or can't decode scanCONTROL type");
			delete m_pLLT;
			return false;
		}
		else if (m_tscanCONTROLType >= scanCONTROL27xx_25 && m_tscanCONTROLType <= scanCONTROL27xx_xxx) cout << endl << "7.Get scanCONTROL type: OK and scanCONTROL27xx";
		else if (m_tscanCONTROLType >= scanCONTROL26xx_25 && m_tscanCONTROLType <= scanCONTROL26xx_xxx) cout << endl << "7.Get scanCONTROL type: OK and scanCONTROL26xx";
		else if (m_tscanCONTROLType >= scanCONTROL29xx_25 && m_tscanCONTROLType <= scanCONTROL29xx_xxx) cout << endl << "7.Get scanCONTROL type: OK and scanCONTROL29xx";
		else
		{
			QMessageBox::warning(this, "", QString("7.Get scanCONTROL type: Err and undefined scanCONTROL type"));
			delete m_pLLT;
			return false;
		}
#endif

#if 1
		//8.Get available resolutions
		ulong vdwResolutions[60];
		if ((iRetValue = m_pLLT->GetResolutions(vdwResolutions, 60)) < GENERAL_FUNCTION_OK)
		{
			QMessageBox::warning(this, "", QString("8.Get available resolutions: Err \nError code : ") + aaa::num2string(iRetValue).c_str());
			delete m_pLLT;
			return false;
		}
		else
		{
			cout << endl << "8.Get available resolutions: OK and " << iRetValue << " resolutions";
			for (int i = 0; i < iRetValue; ++i)
				cout << ": " << vdwResolutions[i];
		}

		//9.Set resolution
		if ((iRetValue = m_pLLT->SetResolution(m_uiResolution = vdwResolutions[0])) < GENERAL_FUNCTION_OK)
		{
			QMessageBox::warning(this, "", QString("9.Set resolution: Err \nError code: ") + aaa::num2string(iRetValue).c_str());
			delete m_pLLT;
			return false;
		}
		else cout << endl << "9.Set resolution: OK";

		//10.Set trigger
		if ((iRetValue = m_pLLT->SetFeature(FEATURE_FUNCTION_TRIGGER, 0x00000000)) < GENERAL_FUNCTION_OK)
		{
			QMessageBox::warning(this, "", QString("10.Set trigger: Err \nError code: ") + aaa::num2string(iRetValue).c_str());
			delete m_pLLT;
			return false;
		}
		else cout << endl << "10.Set trigger: OK";

		//11.Set config
		if ((iRetValue = m_pLLT->SetProfileConfig(PROFILE)) < GENERAL_FUNCTION_OK)
		{
			QMessageBox::warning(this, "", QString("11.Set config: Err \nError code: ") + aaa::num2string(iRetValue).c_str());
			delete m_pLLT;
			return false;
		}
		else cout << endl << "11.Set config: OK";

		//12.Set shuttertime
		if ((iRetValue = m_pLLT->SetFeature(FEATURE_FUNCTION_SHUTTERTIME, uiShutterTime)) < GENERAL_FUNCTION_OK)
		{
			QMessageBox::warning(this, "", QString("12.Set shuttertime: Err \nError code: ") + aaa::num2string(iRetValue).c_str());
			delete m_pLLT;
			return false;
		}
		else cout << endl << "12.Set shuttertime: OK";

		//13.Set idletime
		if ((iRetValue = m_pLLT->SetFeature(FEATURE_FUNCTION_IDLETIME, uiIdleTime)) < GENERAL_FUNCTION_OK)
		{
			QMessageBox::warning(this, "", QString("13.Set idletime: Err \nError code: ") + aaa::num2string(iRetValue).c_str());
			delete m_pLLT;
			return false;
		}
		else cout << endl << "13.Set idletime: OK";

		//14.Transfer profiles
		if ((iRetValue = m_pLLT->TransferProfiles(NORMAL_TRANSFER, true)) < GENERAL_FUNCTION_OK)
		{
			QMessageBox::warning(this, "", QString("14.Transfer profiles: Err \nError code: ") + aaa::num2string(iRetValue).c_str());
			return false;
		}
		else cout << endl << "14.Transfer profiles: OK";
#endif

		return true;
	}

public:
	void pushButtonMoveRight_clickded(){}
	void pushButtonMoveLeft_clickded() {}
	void pushButtonMoveForward_clickded() {}
	void pushButtonMoveBackward_clickded() {}
	void pushButtonMoveUp_clickded() {}
	void pushButtonMoveDown_clickded() {}
	void pushButtonRotateClockwise_clickded() {}
	void pushButtonRotateAntiClockwise_clickded() {}
	void pushButtonReset_clickded() {}
	void PushButtonStartup_clickded() 
	{ 
		if (timerContinousScan->isActive())
		{
			timerContinousScan->stop();
			EnableOrDisableControls(true);
			timeid = -1;
			scanid = -1;
			saveScan = false;
		}
		else
		{
			timerContinousScan->start(100);
			EnableOrDisableControls(false);
			timeid = time(0);
			scanid = 0;
			saveScan = false;

			QSqlQuery query(db);
			query.prepare("insert into tb_scan_catalog (timeid, direction, startpoint, endpoint, step) values (?, ?, ?, ?, ?)");
			query.addBindValue(timeid);
			query.addBindValue(radioButtonXAxis->isChecked() ? "X" : "Y");
			query.addBindValue(spinBoxStartPoint->value());
			query.addBindValue(spinBoxEndPoint->value());
			query.addBindValue(comboBoxStep->currentText().toInt());
			query.exec();
		}
	}
	void timerContinousScan_timeout() { GetAndSaveAndShowProfiles(); }
	void customPlotViewScan_mousePress(QMouseEvent *event) { saveScan = true; }

public:
	//1
	QVBoxLayout *vboxLayoutWorkArea = new QVBoxLayout(this);
	QTabWidget* tabWidgetScanAnalyse = new QTabWidget(this);
	QWidget* widgetScan = new QWidget(tabWidgetScanAnalyse);
	QWidget* widgetAnalyse = new QWidget(tabWidgetScanAnalyse);

	//2
	QGridLayout* gridLayoutWidgetScan = new QGridLayout(widgetScan);
	QCustomPlot* customPlotViewScan = new QCustomPlot(widgetScan);
	QGroupBox* groupBoxCurrentPose = new QGroupBox("Current Pose", widgetScan);
	QGroupBox* groupBoxPoseSetting = new QGroupBox("Pose Setting", widgetScan);
	QGroupBox* groupBoxScanSetting = new QGroupBox("Scan Setting", widgetScan);
	QGroupBox* groupBoxImplement = new QGroupBox("Implement", widgetScan);

	//3
	QGridLayout* gridLayoutGroupBoxCurrentPose = new QGridLayout(groupBoxCurrentPose);
	QLabel* labelXAxis = new QLabel("X Axis", groupBoxCurrentPose);
	QLineEdit* lineEditXAxis = new QLineEdit("-1", groupBoxCurrentPose);
	QLabel* labelXmm = new QLabel("mm", groupBoxCurrentPose);
	QLabel* labelYAxis = new QLabel("Y Axis", groupBoxCurrentPose);
	QLineEdit* lineEditYAxis = new QLineEdit("-1", groupBoxCurrentPose);
	QLabel* labelYmm = new QLabel("mm", groupBoxCurrentPose);
	QLabel* labelZAxis = new QLabel("Z Axis", groupBoxCurrentPose);
	QLineEdit* lineEditZAxis = new QLineEdit("-1", groupBoxCurrentPose);
	QLabel* labelZmm = new QLabel("mm", groupBoxCurrentPose);
	QLabel* labelRotate = new QLabel("Rotate", groupBoxCurrentPose);
	QLineEdit* lineEditRotate = new QLineEdit("-1", groupBoxCurrentPose);
	QLabel* labelRcc = new QLabel(QString::fromLocal8Bit("°"), groupBoxCurrentPose);

	//4
	QGridLayout* gridLayoutPoseSetting = new QGridLayout(groupBoxPoseSetting);
	QPushButton* pushButtonMoveRight = new QPushButton("Move Right", groupBoxPoseSetting);
	QPushButton* pushButtonMoveLeft = new QPushButton("Move Left", groupBoxPoseSetting);
	QPushButton* pushButtonMoveForward = new QPushButton("Move Forward", groupBoxPoseSetting);
	QPushButton* pushButtonMoveBackward = new QPushButton("Move Backward", groupBoxPoseSetting);
	QPushButton* pushButtonMoveUp = new QPushButton("Move Up", groupBoxPoseSetting);
	QPushButton* pushButtonMoveDown = new QPushButton("Move Down", groupBoxPoseSetting);
	QPushButton* pushButtonRotateClockwise = new QPushButton("Move Clockwise", groupBoxPoseSetting);
	QPushButton* pushButtonRotateAntiClockwise = new QPushButton("Move AntiClockwise", groupBoxPoseSetting);

	//5
	QGridLayout* gridLayoutGroupBoxScanSetting = new QGridLayout(groupBoxScanSetting);
	QLabel* labelDirection = new QLabel("Direction", groupBoxScanSetting);
	QRadioButton* radioButtonXAxis = new QRadioButton("X Axis", groupBoxScanSetting);
	QRadioButton* radioButtonYAxis = new QRadioButton("Y Axis", groupBoxScanSetting);
	QLabel* labelStartPoint = new QLabel("Start Point", groupBoxScanSetting);
	QSpinBox *spinBoxStartPoint = new QSpinBox(groupBoxScanSetting);
	QLabel* labelStartPointMM = new QLabel("mm", groupBoxScanSetting);
	QLabel* labelEndPoint = new QLabel("End Point", groupBoxScanSetting);
	QSpinBox *spinBoxEndPoint = new QSpinBox(groupBoxScanSetting);
	QLabel* labelEndPointMM = new QLabel("mm", groupBoxScanSetting);
	QLabel* labelStep = new QLabel("Step", groupBoxScanSetting);
	QLabel* labelStepMM = new QLabel("mm", groupBoxScanSetting);
	QComboBox* comboBoxStep = new QComboBox(groupBoxScanSetting);

	//6
	QGridLayout* gridLayoutGroupBoxImplement = new QGridLayout(groupBoxImplement);
	QPushButton* pushButtonReset = new QPushButton("Reset", groupBoxImplement);
	QPushButton* PushButtonStartup = new QPushButton("Startup", groupBoxImplement);
	QTimer* timerContinousScan = new QTimer(this);

	//7
	QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "connect_name_of_sqlite");

	//8
	QList<QSerialPortInfo> listSerialPortInfo = QSerialPortInfo::availablePorts();
	QSerialPort serialPortXYZR;
	QSerialPort serialPortCtrl;

	//9
	QGridLayout* gridLayoutWidgetAnalyse = new QGridLayout(widgetAnalyse);
	QTableView* tableViewCatalog = new QTableView(widgetAnalyse);
	QTableView* tableViewDetails = new QTableView(widgetAnalyse);
	QSqlTableModel *tableModelCatalog = new QSqlTableModel(widgetAnalyse, db);
	QSqlQueryModel *queryModelDetails = new QSqlQueryModel(widgetAnalyse);
	QCustomPlot* customPlotViewAnlyse = new QCustomPlot(widgetAnalyse);

	MyWidget(QWidget *parent = 0) : QWidget(parent) { createMainUI(); }
	void createMainUI()
	{
		if (initDevice() == false) { QMessageBox::information(this, "", "Fail to connect or open device");  QApplication::exit(); }
		db.setDatabaseName("./../data/mysqlite.db");
		if (!db.open()) { QMessageBox::information(this, "", "Fail to open database, details:\n" + db.lastError().text());  QApplication::exit();}

		//0.
		this->setWindowTitle("Laser Scan");
		this->setWindowIcon(QIcon("./../data/window/boss.ico"));
		this->setMinimumSize(QSize(800, 600));
		this->setFont(QFont("", 20, QFont::Thin));

		//1
		vboxLayoutWorkArea->addWidget(tabWidgetScanAnalyse);
		tabWidgetScanAnalyse->addTab(widgetScan, "Scan");
		tabWidgetScanAnalyse->addTab(widgetAnalyse, "Analyse");
		widgetAnalyse->setFont(QFont("", 10, QFont::Thin));

		//2
		gridLayoutWidgetScan->addWidget(customPlotViewScan, 0, 0, 1, 4);
		gridLayoutWidgetScan->addWidget(groupBoxCurrentPose, 1, 0, 1, 1);
		gridLayoutWidgetScan->addWidget(groupBoxPoseSetting, 1, 1, 1, 1);
		gridLayoutWidgetScan->addWidget(groupBoxScanSetting, 1, 2, 1, 1);
		gridLayoutWidgetScan->addWidget(groupBoxImplement, 1, 3, 1, 1);
		gridLayoutWidgetScan->setColumnStretch(0, 2);
		gridLayoutWidgetScan->setColumnStretch(1, 2);
		gridLayoutWidgetScan->setColumnStretch(2, 2);
		gridLayoutWidgetScan->setColumnStretch(3, 1);
		gridLayoutGroupBoxCurrentPose->addWidget(labelXAxis, 0, 0, 1, 1);
		gridLayoutGroupBoxCurrentPose->addWidget(lineEditXAxis, 0, 1, 1, 1);
		gridLayoutGroupBoxCurrentPose->addWidget(labelXmm, 0, 2, 1, 1);
		gridLayoutGroupBoxCurrentPose->addWidget(labelYAxis, 1, 0, 1, 1);
		gridLayoutGroupBoxCurrentPose->addWidget(lineEditYAxis, 1, 1, 1, 1);
		gridLayoutGroupBoxCurrentPose->addWidget(labelYmm, 1, 2, 1, 1);
		gridLayoutGroupBoxCurrentPose->addWidget(labelZAxis, 2, 0, 1, 1);
		gridLayoutGroupBoxCurrentPose->addWidget(lineEditZAxis, 2, 1, 1, 1);
		gridLayoutGroupBoxCurrentPose->addWidget(labelZmm, 2, 2, 1, 1);
		gridLayoutGroupBoxCurrentPose->addWidget(labelRotate, 3, 0, 1, 1);
		gridLayoutGroupBoxCurrentPose->addWidget(lineEditRotate, 3, 1, 1, 1);
		gridLayoutGroupBoxCurrentPose->addWidget(labelRcc, 3, 2, 1, 1);
		customPlotViewScan->addGraph();
		lineEditXAxis->setEnabled(false);
		lineEditYAxis->setEnabled(false);
		lineEditZAxis->setEnabled(false);
		lineEditRotate->setEnabled(false);

		//4.
		gridLayoutPoseSetting->addWidget(pushButtonMoveRight, 0, 0, 1, 1);
		gridLayoutPoseSetting->addWidget(pushButtonMoveLeft, 0, 1, 1, 1);
		gridLayoutPoseSetting->addWidget(pushButtonMoveForward, 1, 0, 1, 1);
		gridLayoutPoseSetting->addWidget(pushButtonMoveBackward, 1, 1, 1, 1);
		gridLayoutPoseSetting->addWidget(pushButtonMoveUp, 2, 0, 1, 1);
		gridLayoutPoseSetting->addWidget(pushButtonMoveDown, 2, 1, 1, 1);
		gridLayoutPoseSetting->addWidget(pushButtonRotateClockwise, 3, 0, 1, 1);
		gridLayoutPoseSetting->addWidget(pushButtonRotateAntiClockwise, 3, 1, 1, 1);

		//5.
		groupBoxScanSetting->setLayout(gridLayoutGroupBoxScanSetting);
		gridLayoutGroupBoxScanSetting->addWidget(labelDirection, 0, 0, 1, 1);
		gridLayoutGroupBoxScanSetting->addWidget(radioButtonXAxis, 0, 1, 1, 1);
		gridLayoutGroupBoxScanSetting->addWidget(radioButtonYAxis, 0, 2, 1, 1);
		gridLayoutGroupBoxScanSetting->addWidget(labelStartPoint, 1, 0, 1, 1);
		gridLayoutGroupBoxScanSetting->addWidget(spinBoxStartPoint, 1, 1, 1, 2);
		gridLayoutGroupBoxScanSetting->addWidget(labelStartPointMM, 1, 3, 1, 1);
		gridLayoutGroupBoxScanSetting->addWidget(labelEndPoint, 2, 0, 1, 1);
		gridLayoutGroupBoxScanSetting->addWidget(spinBoxEndPoint, 2, 1, 1, 2);
		gridLayoutGroupBoxScanSetting->addWidget(labelEndPointMM, 2, 3, 1, 1);
		gridLayoutGroupBoxScanSetting->addWidget(labelStep, 3, 0, 1, 1);
		gridLayoutGroupBoxScanSetting->addWidget(comboBoxStep, 3, 1, 1, 2);
		gridLayoutGroupBoxScanSetting->addWidget(labelStepMM, 3, 3, 1, 1);
		spinBoxStartPoint->setMinimum(-100);
		spinBoxEndPoint->setMinimum(-100);
		comboBoxStep->addItem("1");
		comboBoxStep->addItem("10");
		radioButtonXAxis->setChecked(true);

		//6.
		gridLayoutGroupBoxImplement->addWidget(pushButtonReset, 0, 0, 1, 1);
		gridLayoutGroupBoxImplement->addWidget(PushButtonStartup, 1, 0, 1, 1);
		pushButtonReset->setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred));
		PushButtonStartup->setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred));

		//7.
		connect(pushButtonMoveRight, &QPushButton::clicked, this, &MyWidget::pushButtonMoveRight_clickded);
		connect(pushButtonMoveLeft, &QPushButton::clicked, this, &MyWidget::pushButtonMoveLeft_clickded);
		connect(pushButtonMoveForward, &QPushButton::clicked, this, &MyWidget::pushButtonMoveForward_clickded);
		connect(pushButtonMoveBackward, &QPushButton::clicked, this, &MyWidget::pushButtonMoveBackward_clickded);
		connect(pushButtonMoveUp, &QPushButton::clicked, this, &MyWidget::pushButtonMoveUp_clickded);
		connect(pushButtonMoveDown, &QPushButton::clicked, this, &MyWidget::pushButtonMoveDown_clickded);
		connect(pushButtonRotateClockwise, &QPushButton::clicked, this, &MyWidget::pushButtonRotateClockwise_clickded);
		connect(pushButtonRotateAntiClockwise, &QPushButton::clicked, this, &MyWidget::pushButtonRotateAntiClockwise_clickded);
		connect(pushButtonReset, &QPushButton::clicked, this, &MyWidget::pushButtonReset_clickded);
		connect(PushButtonStartup, &QPushButton::clicked, this, &MyWidget::PushButtonStartup_clickded);
		connect(timerContinousScan, &QTimer::timeout, this, &MyWidget::timerContinousScan_timeout);
		connect(customPlotViewScan, &QCustomPlot::mousePress, this, &MyWidget::customPlotViewScan_mousePress);

		
		connect(&serialPortXYZR, &QSerialPort::readyRead, this, &MyWidget::serialPortXYZR_readyRead);
		connect(&serialPortXYZR, &QSerialPort::bytesWritten, this, &MyWidget::serialPortXYZR_bytesWritten);
		connect(&serialPortXYZR, static_cast<void(QSerialPort::*)(QSerialPort::SerialPortError)>(&QSerialPort::error), this, &MyWidget::serialPortXYZR_error);
		connect(&serialPortCtrl, &QSerialPort::readyRead, this, &MyWidget::serialPortCtrl_readyRead);
		connect(&serialPortCtrl, &QSerialPort::bytesWritten, this, &MyWidget::serialPortCtrl_bytesWritten);
		connect(&serialPortCtrl, static_cast<void(QSerialPort::*)(QSerialPort::SerialPortError)>(&QSerialPort::error), this, &MyWidget::serialPortCtrl_error);
		serialPortXYZR.setPort(listSerialPortInfo[1]);
		serialPortXYZR.setBaudRate(460800);
		serialPortXYZR.open(QIODevice::ReadWrite);
		cout << endl << serialPortXYZR.portName().toStdString();

		//8.
		gridLayoutWidgetAnalyse->addWidget(tableViewCatalog, 0, 0);
		gridLayoutWidgetAnalyse->addWidget(tableViewDetails, 0, 1);
		gridLayoutWidgetAnalyse->addWidget(customPlotViewAnlyse, 1, 0, 1, 2);
		gridLayoutWidgetAnalyse->setRowStretch(0, 2);
		gridLayoutWidgetAnalyse->setRowStretch(1, 3);
		gridLayoutWidgetAnalyse->setColumnStretch(0, 3);
		gridLayoutWidgetAnalyse->setColumnStretch(1, 2);
		customPlotViewAnlyse->addGraph();

		tableViewCatalog->setSelectionBehavior(QAbstractItemView::SelectRows);
		tableViewDetails->setSelectionBehavior(QAbstractItemView::SelectRows);

		tableModelCatalog->setTable("tb_scan_catalog");
		tableModelCatalog->sort(0, Qt::DescendingOrder);
		tableModelCatalog->setEditStrategy(QSqlTableModel::OnManualSubmit);
		if (!tableModelCatalog->select()) { QMessageBox::information(this, "", tableModelCatalog->lastError().text()); return; }
		tableViewCatalog->setModel(tableModelCatalog);

		queryModelDetails->sort(0, Qt::DescendingOrder);
		//queryModelDetails->setQuery("select timeid, scanid from tb_scan_details", db);
		tableViewDetails->setModel(queryModelDetails);

		connect(tableViewCatalog, &QTableView::clicked, this, &MyWidget::tableViewCatalog_clicked);
		connect(tableViewDetails, &QTableView::clicked, this, &MyWidget::tableViewDetails_clicked);
	}


//Data analyse module
public:
	void tableViewCatalog_clicked(QModelIndex modelIndex)
	{
		QSqlRecord record = tableModelCatalog->record(modelIndex.row());
		queryModelDetails->setQuery("select * from tb_scan_details where timeid=" + record.value(0).toString(), db);
	}
	void tableViewDetails_clicked(QModelIndex modelIndex)
	{
		QSqlRecord record = queryModelDetails->record(modelIndex.row());
		vector<double> vdValueX(record.value(2).toByteArray().size() / sizeof(double));
		vector<double> vdValueZ(record.value(3).toByteArray().size() / sizeof(double));
		memcpy(&vdValueX[0], record.value(2).toByteArray().data(), record.value(2).toByteArray().size());
		memcpy(&vdValueZ[0], record.value(3).toByteArray().data(), record.value(3).toByteArray().size());

		vector<double>::iterator minX = min_element(std::begin(vdValueX), std::end(vdValueX));
		vector<double>::iterator maxX = max_element(std::begin(vdValueX), std::end(vdValueX));
		vector<double>::iterator minZ = min_element(std::begin(vdValueZ), std::end(vdValueZ));
		vector<double>::iterator maxZ = max_element(std::begin(vdValueZ), std::end(vdValueZ));
		QVector<double> vdValueXX = QVector<double>::fromStdVector(vdValueX);
		QVector<double> vdValueZZ = QVector<double>::fromStdVector(vdValueZ);
		customPlotViewAnlyse->xAxis->setRange(*minX, *maxX);
		customPlotViewAnlyse->yAxis->setRange(*minZ, *maxZ);
		customPlotViewAnlyse->graph(0)->setData(vdValueXX, vdValueZZ);
		customPlotViewAnlyse->replot();
	}
};


#endif