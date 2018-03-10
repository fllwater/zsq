#include "window/windowaaa.h"
#include "InterfaceLLT_2.h"
#include "qcustomplot.h"


#ifndef __demowin_h__
#define __demowin_h__


class MyWidget : public QWidget
{
public:
	CInterfaceLLT* m_pLLT = NULL;
	TScannerType m_tscanCONTROLType;
	uint m_uiResolution = 0;
	int iRetValue = 0;//buffer
public:
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

	void GetAndSaveAndShowProfiles(bool isSave = false)
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
		if (isSave)
		{

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



public:
	void pushButtonMoveRight_clickded(){}
	void pushButtonMoveLeft_clickded() {}
	void pushButtonMoveForward_clickded() {}
	void pushButtonMoveBackward_clickded() {}
	void pushButtonMoveUp_clickded() {}
	void pushButtonMoveDown_clickded() {}
	void pushButtonRotateClockwise_clickded() {}
	void pushButtonRotateAntiClockwise_clickded() {}
	void pushButtonReset_clickded() 
	{
		if (timerTestContinousScan->isActive()) timerTestContinousScan->stop(); 
		else timerTestContinousScan->start(100);
	}
	void PushButtonStartup_clickded() { GetAndSaveAndShowProfiles(true); }
	void timerTestContinousScan_timeout() { GetAndSaveAndShowProfiles(false); }
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
	QLineEdit* lineEditStartPoint = new QLineEdit("0", groupBoxScanSetting);
	QLabel* labelStartPointMM = new QLabel("mm", groupBoxScanSetting);
	QLabel* labelEndPoint = new QLabel("End Point", groupBoxScanSetting);
	QLineEdit* lineEditEndPoint = new QLineEdit("0", groupBoxScanSetting);
	QLabel* labelEndPointMM = new QLabel("mm", groupBoxScanSetting);
	QLabel* labelStep = new QLabel("Step", groupBoxScanSetting);
	QLabel* labelStepMM = new QLabel("mm", groupBoxScanSetting);
	QComboBox* comboBoxStep = new QComboBox(groupBoxScanSetting);

	//6
	QGridLayout* gridLayoutGroupBoxImplement = new QGridLayout(groupBoxImplement);
	QPushButton* pushButtonReset = new QPushButton("Reset", groupBoxImplement);
	QPushButton* PushButtonStartup = new QPushButton("Startup", groupBoxImplement);
	QTimer* timerTestContinousScan = new QTimer(this);

	//7
	QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "connect_name_of_sqlite");
	QSqlQuery *query = new QSqlQuery("", db);

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
		gridLayoutGroupBoxScanSetting->addWidget(lineEditStartPoint, 1, 1, 1, 2);
		gridLayoutGroupBoxScanSetting->addWidget(labelStartPointMM, 1, 3, 1, 1);
		gridLayoutGroupBoxScanSetting->addWidget(labelEndPoint, 2, 0, 1, 1);
		gridLayoutGroupBoxScanSetting->addWidget(lineEditEndPoint, 2, 1, 1, 2);
		gridLayoutGroupBoxScanSetting->addWidget(labelEndPointMM, 2, 3, 1, 1);
		gridLayoutGroupBoxScanSetting->addWidget(labelStep, 3, 0, 1, 1);
		gridLayoutGroupBoxScanSetting->addWidget(comboBoxStep, 3, 1, 1, 2);
		gridLayoutGroupBoxScanSetting->addWidget(labelStepMM, 3, 3, 1, 1);
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
		connect(timerTestContinousScan, &QTimer::timeout, this, &MyWidget::timerTestContinousScan_timeout);
	}

};


#endif