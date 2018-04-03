#include "window/windowaaa.h"
#include "InterfaceLLT_2.h"
#include "qcustomplot.h"
#include "CommDef.h"


#ifndef __demowin_h__
#define __demowin_h__


class MyWidget : public QWidget
{
public:
	MyWidget(QWidget *parent = 0) : QWidget(parent) {createMainUI(); }
	
public://Create UI
#if 1
	QVBoxLayout *vboxLayoutWorkArea = new QVBoxLayout(this);//addin1
	QTabWidget *tabWidgetScanAnalyse = new QTabWidget(this);//addin2
		QWidget *widgetScan = new QWidget(tabWidgetScanAnalyse);
			QGridLayout *gridLayoutWidgetScan = new QGridLayout(widgetScan);//addin3
			QCustomPlot *customPlotViewScan = new QCustomPlot(widgetScan);
			QGroupBox *groupBoxPortSetting = new QGroupBox("Communication Setting", widgetScan);
				QGridLayout *gridLayoutGroupBoxPortSetting = new QGridLayout(groupBoxPortSetting);//addin4
				QComboBox *comboBoxDataPort = new QComboBox(groupBoxPortSetting);
				QPushButton *pushButtonOpenDataPort = new QPushButton("Open", groupBoxPortSetting);
				QComboBox *comboBoxCtrlPort = new QComboBox(groupBoxPortSetting);
				QPushButton *pushButtonOpenCtrlPort = new QPushButton("Open", groupBoxPortSetting);
				QPushButton *pushButtonOpenEthernet = new QPushButton("Open Ethernet", groupBoxPortSetting);
				QList<QSerialPortInfo> listSerialPortInfo = QSerialPortInfo::availablePorts();//special
			QGroupBox *groupBoxCurrentPose = new QGroupBox("Current Pose", widgetScan);
				QGridLayout *gridLayoutGroupBoxCurrentPose = new QGridLayout(groupBoxCurrentPose);//addin5
				QLineEdit *lineEditXAxis = new QLineEdit("-1.000", groupBoxCurrentPose);
				QLineEdit *lineEditYAxis = new QLineEdit("-1.000", groupBoxCurrentPose);
				QLineEdit *lineEditZAxis = new QLineEdit("-1.000", groupBoxCurrentPose);
				QLineEdit *lineEditRotate = new QLineEdit("-1.000", groupBoxCurrentPose);
			QGroupBox *groupBoxPoseSetting = new QGroupBox("Pose Setting", widgetScan);
				QGridLayout *gridLayoutPoseSetting = new QGridLayout(groupBoxPoseSetting);//addin6
				QPushButton *pushButtonMoveRight = new QPushButton("Move Right", groupBoxPoseSetting);
				QPushButton *pushButtonMoveLeft = new QPushButton("Move Left", groupBoxPoseSetting);
				QPushButton *pushButtonMoveForward = new QPushButton("Move Forward", groupBoxPoseSetting);
				QPushButton *pushButtonMoveBackward = new QPushButton("Move Backward", groupBoxPoseSetting);
				QPushButton *pushButtonMoveUp = new QPushButton("Move Up", groupBoxPoseSetting);
				QPushButton *pushButtonMoveDown = new QPushButton("Move Down", groupBoxPoseSetting);
				QPushButton *pushButtonRotateClockwise = new QPushButton("Move Clockwise", groupBoxPoseSetting);
				QPushButton *pushButtonRotateAntiClockwise = new QPushButton("Move AntiClockwise", groupBoxPoseSetting);
			QGroupBox *groupBoxScanSetting = new QGroupBox("Scan Setting", widgetScan);
				QGridLayout *gridLayoutGroupBoxScanSetting = new QGridLayout(groupBoxScanSetting);//addin7
				QRadioButton *radioButtonXAxis = new QRadioButton("X Axis", groupBoxScanSetting);
				QRadioButton *radioButtonYAxis = new QRadioButton("Y Axis", groupBoxScanSetting);
				QSpinBox *spinBoxStartPoint = new QSpinBox(groupBoxScanSetting);
				QSpinBox *spinBoxEndPoint = new QSpinBox(groupBoxScanSetting);
				QComboBox *comboBoxStep = new QComboBox(groupBoxScanSetting);
			QGroupBox *groupBoxImplement = new QGroupBox("Implement", widgetScan);
				QGridLayout *gridLayoutGroupBoxImplement = new QGridLayout(groupBoxImplement);//addin8
				QPushButton *pushButtonReset = new QPushButton("Reset", groupBoxImplement);
				QPushButton *PushButtonStartup = new QPushButton("Startup", groupBoxImplement);
		QWidget *widgetAnalyse = new QWidget(tabWidgetScanAnalyse);
			QGridLayout *gridLayoutWidgetAnalyse = new QGridLayout(widgetAnalyse);//addin9
			QTableView *tableViewCatalog = new QTableView(widgetAnalyse);
			QTableView *tableViewDetails = new QTableView(widgetAnalyse);
			QCustomPlot *customPlotViewAnlyse = new QCustomPlot(widgetAnalyse);
			QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "connect_name_of_sqlite");//addin10
			QSqlTableModel *tableModelCatalog = new QSqlTableModel(widgetAnalyse, db);
			QSqlQueryModel *queryModelDetails = new QSqlQueryModel(widgetAnalyse);		
#endif
	void createMainUI()
	{
		//0.
		this->setWindowTitle("Laser Scan");
		this->setWindowIcon(QIcon("./../data/window/boss.ico"));
		this->setMinimumSize(QSize(800, 600));
		this->setFont(QFont("", 20, QFont::Thin));

		//addin1
		vboxLayoutWorkArea->addWidget(tabWidgetScanAnalyse);

		//addin2
		tabWidgetScanAnalyse->addTab(widgetScan, "Scan");
		tabWidgetScanAnalyse->addTab(widgetAnalyse, "Analyse"); widgetAnalyse->setFont(QFont("", 10, QFont::Thin));

		//addin3
		gridLayoutWidgetScan->addWidget(customPlotViewScan, 0, 0, 1, 4); customPlotViewScan->addGraph();
		gridLayoutWidgetScan->addWidget(groupBoxPortSetting, 1, 0, 1, 4);
		gridLayoutWidgetScan->addWidget(groupBoxCurrentPose, 2, 0, 1, 1);
		gridLayoutWidgetScan->addWidget(groupBoxPoseSetting, 2, 1, 1, 1);
		gridLayoutWidgetScan->addWidget(groupBoxScanSetting, 2, 2, 1, 1);
		gridLayoutWidgetScan->addWidget(groupBoxImplement, 2, 3, 1, 1);
		gridLayoutWidgetScan->setRowStretch(0, 7);
		gridLayoutWidgetScan->setRowStretch(1, 1);
		gridLayoutWidgetScan->setRowStretch(2, 2);
		gridLayoutWidgetScan->setColumnStretch(0, 2);
		gridLayoutWidgetScan->setColumnStretch(1, 2);
		gridLayoutWidgetScan->setColumnStretch(2, 2);
		gridLayoutWidgetScan->setColumnStretch(3, 1);
		connect(customPlotViewScan, &QCustomPlot::mousePress, this, &MyWidget::customPlotViewScan_mousePress);

		//addin4
		gridLayoutGroupBoxPortSetting->addWidget(new QLabel("Data SerialPort", groupBoxPortSetting), 0, 0, 1, 1);
		gridLayoutGroupBoxPortSetting->addWidget(comboBoxDataPort, 0, 1, 1, 1); for (int i = 0; i < listSerialPortInfo.size(); ++i) comboBoxDataPort->addItem(listSerialPortInfo[i].portName());
		gridLayoutGroupBoxPortSetting->addWidget(pushButtonOpenDataPort, 0, 2, 1, 1);
		gridLayoutGroupBoxPortSetting->addWidget(new QLabel("Ctrl SerialPort", groupBoxPortSetting), 0, 4, 1, 1);
		gridLayoutGroupBoxPortSetting->addWidget(comboBoxCtrlPort, 0, 5, 1, 1); for (int i = 0; i < listSerialPortInfo.size(); ++i) comboBoxCtrlPort->addItem(listSerialPortInfo[i].portName());
		gridLayoutGroupBoxPortSetting->addWidget(pushButtonOpenCtrlPort, 0, 6, 1, 1);
		gridLayoutGroupBoxPortSetting->addWidget(pushButtonOpenEthernet, 0, 8, 1, 1);
		gridLayoutGroupBoxPortSetting->setColumnStretch(0, 0);
		gridLayoutGroupBoxPortSetting->setColumnStretch(1, 2);
		gridLayoutGroupBoxPortSetting->setColumnStretch(2, 1);
		gridLayoutGroupBoxPortSetting->setColumnStretch(3, 1);
		gridLayoutGroupBoxPortSetting->setColumnStretch(4, 0);
		gridLayoutGroupBoxPortSetting->setColumnStretch(5, 2);
		gridLayoutGroupBoxPortSetting->setColumnStretch(6, 1);
		gridLayoutGroupBoxPortSetting->setColumnStretch(7, 1);
		gridLayoutGroupBoxPortSetting->setColumnStretch(8, 1);
		connect(pushButtonOpenDataPort, &QPushButton::clicked, this, &MyWidget::pushButtonOpenDataPort_clickded);
		connect(pushButtonOpenCtrlPort, &QPushButton::clicked, this, &MyWidget::pushButtonOpenCtrlPort_clickded);
		connect(pushButtonOpenEthernet, &QPushButton::clicked, this, &MyWidget::pushButtonOpenEthernet_clickded);

		//addin5
		gridLayoutGroupBoxCurrentPose->addWidget(new QLabel("X Axis", groupBoxCurrentPose), 0, 0, 1, 1);
		gridLayoutGroupBoxCurrentPose->addWidget(lineEditXAxis, 0, 1, 1, 1); lineEditXAxis->setEnabled(false);
		gridLayoutGroupBoxCurrentPose->addWidget(new QLabel("mm", groupBoxCurrentPose), 0, 2, 1, 1);
		gridLayoutGroupBoxCurrentPose->addWidget(new QLabel("Y Axis", groupBoxCurrentPose), 1, 0, 1, 1);
		gridLayoutGroupBoxCurrentPose->addWidget(lineEditYAxis, 1, 1, 1, 1); lineEditYAxis->setEnabled(false);
		gridLayoutGroupBoxCurrentPose->addWidget(new QLabel("mm", groupBoxCurrentPose), 1, 2, 1, 1);
		gridLayoutGroupBoxCurrentPose->addWidget(new QLabel("Z Axis", groupBoxCurrentPose), 2, 0, 1, 1);
		gridLayoutGroupBoxCurrentPose->addWidget(lineEditZAxis, 2, 1, 1, 1); lineEditZAxis->setEnabled(false);
		gridLayoutGroupBoxCurrentPose->addWidget(new QLabel("mm", groupBoxCurrentPose), 2, 2, 1, 1);
		gridLayoutGroupBoxCurrentPose->addWidget(new QLabel("Rotate", groupBoxCurrentPose), 3, 0, 1, 1);
		gridLayoutGroupBoxCurrentPose->addWidget(lineEditRotate, 3, 1, 1, 1); lineEditRotate->setEnabled(false);
		gridLayoutGroupBoxCurrentPose->addWidget(new QLabel(QString::fromLocal8Bit("°"), groupBoxCurrentPose), 3, 2, 1, 1);

		//addin6
		gridLayoutPoseSetting->addWidget(pushButtonMoveRight, 0, 0, 1, 1);
		gridLayoutPoseSetting->addWidget(pushButtonMoveLeft, 0, 1, 1, 1);
		gridLayoutPoseSetting->addWidget(pushButtonMoveForward, 1, 0, 1, 1);
		gridLayoutPoseSetting->addWidget(pushButtonMoveBackward, 1, 1, 1, 1);
		gridLayoutPoseSetting->addWidget(pushButtonMoveUp, 2, 0, 1, 1);
		gridLayoutPoseSetting->addWidget(pushButtonMoveDown, 2, 1, 1, 1);
		gridLayoutPoseSetting->addWidget(pushButtonRotateClockwise, 3, 0, 1, 1);
		gridLayoutPoseSetting->addWidget(pushButtonRotateAntiClockwise, 3, 1, 1, 1);
		connect(pushButtonMoveRight, &QPushButton::pressed, this, &MyWidget::pushButtonMoveRight_pressed);
		connect(pushButtonMoveLeft, &QPushButton::pressed, this, &MyWidget::pushButtonMoveLeft_pressed);
		connect(pushButtonMoveForward, &QPushButton::pressed, this, &MyWidget::pushButtonMoveForward_pressed);
		connect(pushButtonMoveBackward, &QPushButton::pressed, this, &MyWidget::pushButtonMoveBackward_pressed);
		connect(pushButtonMoveUp, &QPushButton::pressed, this, &MyWidget::pushButtonMoveUp_pressed);
		connect(pushButtonMoveDown, &QPushButton::pressed, this, &MyWidget::pushButtonMoveDown_pressed);
		connect(pushButtonRotateClockwise, &QPushButton::pressed, this, &MyWidget::pushButtonRotateClockwise_pressed);
		connect(pushButtonRotateAntiClockwise, &QPushButton::pressed, this, &MyWidget::pushButtonRotateAntiClockwise_pressed);
		connect(pushButtonMoveRight, &QPushButton::released, this, &MyWidget::pushButtonMoveRight_released);
		connect(pushButtonMoveLeft, &QPushButton::released, this, &MyWidget::pushButtonMoveLeft_released);
		connect(pushButtonMoveForward, &QPushButton::released, this, &MyWidget::pushButtonMoveForward_released);
		connect(pushButtonMoveBackward, &QPushButton::released, this, &MyWidget::pushButtonMoveBackward_released);
		connect(pushButtonMoveUp, &QPushButton::released, this, &MyWidget::pushButtonMoveUp_released);
		connect(pushButtonMoveDown, &QPushButton::released, this, &MyWidget::pushButtonMoveDown_released);
		connect(pushButtonRotateClockwise, &QPushButton::released, this, &MyWidget::pushButtonRotateClockwise_released);
		connect(pushButtonRotateAntiClockwise, &QPushButton::released, this, &MyWidget::pushButtonRotateAntiClockwise_released);

		//addin7
		groupBoxScanSetting->setLayout(gridLayoutGroupBoxScanSetting);
		gridLayoutGroupBoxScanSetting->addWidget(new QLabel("Direction", groupBoxScanSetting), 0, 0, 1, 1);
		gridLayoutGroupBoxScanSetting->addWidget(radioButtonXAxis, 0, 1, 1, 1); radioButtonXAxis->setChecked(true);
		gridLayoutGroupBoxScanSetting->addWidget(radioButtonYAxis, 0, 2, 1, 1);
		gridLayoutGroupBoxScanSetting->addWidget(new QLabel("Start Point", groupBoxScanSetting), 1, 0, 1, 1);
		gridLayoutGroupBoxScanSetting->addWidget(spinBoxStartPoint, 1, 1, 1, 2); spinBoxStartPoint->setMinimum(-100);
		gridLayoutGroupBoxScanSetting->addWidget(new QLabel("mm", groupBoxScanSetting), 1, 3, 1, 1);
		gridLayoutGroupBoxScanSetting->addWidget(new QLabel("End Point", groupBoxScanSetting), 2, 0, 1, 1);
		gridLayoutGroupBoxScanSetting->addWidget(spinBoxEndPoint, 2, 1, 1, 2); spinBoxEndPoint->setMinimum(-100);
		gridLayoutGroupBoxScanSetting->addWidget(new QLabel("mm", groupBoxScanSetting), 2, 3, 1, 1);
		gridLayoutGroupBoxScanSetting->addWidget(new QLabel("Step", groupBoxScanSetting), 3, 0, 1, 1);
		gridLayoutGroupBoxScanSetting->addWidget(comboBoxStep, 3, 1, 1, 2); comboBoxStep->addItems(QStringList() << "1" << "10");
		gridLayoutGroupBoxScanSetting->addWidget(new QLabel("mm", groupBoxScanSetting), 3, 3, 1, 1);

		//addin8
		gridLayoutGroupBoxImplement->addWidget(pushButtonReset, 0, 0, 1, 1); pushButtonReset->setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred));
		gridLayoutGroupBoxImplement->addWidget(PushButtonStartup, 1, 0, 1, 1); PushButtonStartup->setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred));
		connect(pushButtonReset, &QPushButton::clicked, this, &MyWidget::pushButtonReset_clicked);
		connect(PushButtonStartup, &QPushButton::clicked, this, &MyWidget::PushButtonStartup_clickded);

		//addin9
		gridLayoutWidgetAnalyse->addWidget(tableViewCatalog, 0, 0); tableViewCatalog->setSelectionBehavior(QAbstractItemView::SelectRows);
		gridLayoutWidgetAnalyse->addWidget(tableViewDetails, 0, 1); tableViewDetails->setSelectionBehavior(QAbstractItemView::SelectRows);
		gridLayoutWidgetAnalyse->addWidget(customPlotViewAnlyse, 1, 0, 1, 2); customPlotViewAnlyse->addGraph();
		gridLayoutWidgetAnalyse->setRowStretch(0, 2);
		gridLayoutWidgetAnalyse->setRowStretch(1, 3);
		gridLayoutWidgetAnalyse->setColumnStretch(0, 3);
		gridLayoutWidgetAnalyse->setColumnStretch(1, 2);
		connect(tableViewCatalog, &QTableView::clicked, this, &MyWidget::tableViewCatalog_clicked);
		connect(tableViewDetails, &QTableView::clicked, this, &MyWidget::tableViewDetails_clicked);

		//addin10
		db.setDatabaseName("./../data/mysqlite.db");
		if (!db.open()) { QMessageBox::information(this, "", "Fail to open database, details:\n" + db.lastError().text());  QApplication::exit(); }

		tableModelCatalog->setTable("tb_scan_catalog");
		tableModelCatalog->sort(0, Qt::DescendingOrder);
		tableModelCatalog->setEditStrategy(QSqlTableModel::OnManualSubmit);
		if (!tableModelCatalog->select()) { QMessageBox::information(this, "", tableModelCatalog->lastError().text()); return; }
		tableViewCatalog->setModel(tableModelCatalog);

		queryModelDetails->sort(0, Qt::DescendingOrder);
		//queryModelDetails->setQuery("select timeid, scanid from tb_scan_details", db);
		tableViewDetails->setModel(queryModelDetails);
	}
	
public://Init serialport
	QSerialPort serialPortXYZR;
	QSerialPort serialPortCtrl;
	void pushButtonOpenDataPort_clickded() 
	{
		if (serialPortXYZR.isOpen())
		{
			serialPortXYZR.close();
			pushButtonOpenDataPort->setText("Open");
			disconnect(&serialPortXYZR, &QSerialPort::readyRead, this, &MyWidget::serialPortXYZR_readyRead);
			disconnect(&serialPortXYZR, &QSerialPort::bytesWritten, this, &MyWidget::serialPortXYZR_bytesWritten);
			disconnect(&serialPortXYZR, static_cast<void(QSerialPort::*)(QSerialPort::SerialPortError)>(&QSerialPort::error), this, &MyWidget::serialPortXYZR_error);
		}
		else
		{
			serialPortXYZR.setPort(QSerialPortInfo(comboBoxDataPort->currentText()));
			serialPortXYZR.setBaudRate(460800);
			if (serialPortXYZR.open(QIODevice::ReadWrite))
			{
				pushButtonOpenDataPort->setText("Close");
				connect(&serialPortXYZR, &QSerialPort::readyRead, this, &MyWidget::serialPortXYZR_readyRead);
				connect(&serialPortXYZR, &QSerialPort::bytesWritten, this, &MyWidget::serialPortXYZR_bytesWritten);
				connect(&serialPortXYZR, static_cast<void(QSerialPort::*)(QSerialPort::SerialPortError)>(&QSerialPort::error), this, &MyWidget::serialPortXYZR_error);
			}
			else QMessageBox::information(this, "", serialPortXYZR.portName() + " failed to be opened");
		}
	}
	void pushButtonOpenCtrlPort_clickded() 
	{
		if (serialPortCtrl.isOpen())
		{
			serialPortCtrl.close();
			pushButtonOpenCtrlPort->setText("Open");
			disconnect(&serialPortCtrl, &QSerialPort::readyRead, this, &MyWidget::serialPortCtrl_readyRead);
			disconnect(&serialPortCtrl, &QSerialPort::bytesWritten, this, &MyWidget::serialPortCtrl_bytesWritten);
			disconnect(&serialPortCtrl, static_cast<void(QSerialPort::*)(QSerialPort::SerialPortError)>(&QSerialPort::error), this, &MyWidget::serialPortCtrl_error);
		}
		else
		{
			serialPortCtrl.setPort(QSerialPortInfo(comboBoxCtrlPort->currentText()));
			serialPortCtrl.setBaudRate(460800);
			if (serialPortCtrl.open(QIODevice::ReadWrite))
			{
				pushButtonOpenCtrlPort->setText("Close");
				connect(&serialPortCtrl, &QSerialPort::readyRead, this, &MyWidget::serialPortCtrl_readyRead);
				connect(&serialPortCtrl, &QSerialPort::bytesWritten, this, &MyWidget::serialPortCtrl_bytesWritten);
				connect(&serialPortCtrl, static_cast<void(QSerialPort::*)(QSerialPort::SerialPortError)>(&QSerialPort::error), this, &MyWidget::serialPortCtrl_error);
			}
			else QMessageBox::information(this, "", serialPortCtrl.portName() + " failed to be opened");
		}
	}

public://Init ethernet
	CInterfaceLLT *m_pLLT = NULL;
	TScannerType m_tscanCONTROLType;
	uint m_uiResolution = 0;
	QTimer *timerContinousScan = new QTimer(this);
	bool initDevice(uint uiShutterTime = 100, uint uiIdleTime = 900)
	{
		int iRetValue = 0;//buffer
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
	void pushButtonOpenEthernet_clickded()
	{
		if (m_pLLT == NULL)
		{
			if (initDevice()) 
			{ 
				pushButtonOpenEthernet->setText("Close");
				connect(timerContinousScan, &QTimer::timeout, this, &MyWidget::timerContinousScan_timeout); 
			}
			else QMessageBox::information(this, "", "Fail to connect or open device"); 
		}
		else
		{
			delete m_pLLT;
			m_tscanCONTROLType = TScannerType::StandardType;
			m_uiResolution = 0;
			pushButtonOpenEthernet->setText("Open");
			disconnect(timerContinousScan, &QTimer::timeout, this, &MyWidget::timerContinousScan_timeout);
		}
	}
	void timerContinousScan_timeout() { GetAndSaveAndShowProfiles(); }

public://Read serialportXYZR
	queue<uchar> queueXYZR;
	float realtimeX = FLT_MIN;
	float realtimeY = FLT_MIN;
	float realtimeZ = FLT_MIN;
	float realtimeR = FLT_MIN;
	void serialPortXYZR_readyRead()
	{
		//1.
		QByteArray xyz = serialPortXYZR.readAll();
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
		realtimeX = ((int*)(data + 1))[0] / 1000000.f;
		realtimeY = ((int*)(data + 1))[1] / 1000000.f;
		realtimeZ = ((int*)(data + 1))[2] / 1000000.f;
		realtimeR = ((int*)(data + 1))[3] / 1000000.f;
		lineEditXAxis->setText(aaa::num2string(realtimeX, 3).c_str());
		lineEditYAxis->setText(aaa::num2string(realtimeY, 3).c_str());
		lineEditZAxis->setText(aaa::num2string(realtimeZ, 3).c_str());
		lineEditRotate->setText(aaa::num2string(realtimeR, 3).c_str());
	}
	void serialPortXYZR_bytesWritten() {/*no use*/}
	void serialPortXYZR_error(QSerialPort::SerialPortError error) { if (error > 0) QMessageBox::warning(this, "Warning", QString("Error occured and the error code: ") + aaa::num2string(error).c_str()); }

public://Read serialportCtrl
	queue<uchar> queueCtrl;
	CMDCODE slaveCmdCode = CMD_INVALID;
	void serialPortCtrl_readyRead() 
	{
		//1.
		QByteArray cmd = serialPortCtrl.readAll();
		for (int i = 0; i < cmd.size(); ++i) queueCtrl.push((uchar)cmd[i]);

		//2.
		while (queueCtrl.size() >= 12 && queueCtrl.front() != 0x53) queueCtrl.pop();

		//3.
		if (queueCtrl.size() < 12) return;

		//4.
		uchar data[10], sumCalc = (uchar)0;
		queueCtrl.pop();
		for (int i = 0; i < 10; ++i)
		{
			data[i] = queueCtrl.front();
			queueCtrl.pop();
			sumCalc += data[i];
		}
		uchar sumTrue = queueCtrl.front();
		queueCtrl.pop();

		//5.
		if (sumCalc != sumTrue) slaveCmdCode = CMD_INVALID;
		else slaveCmdCode = (CMDCODE)data[0];
	}
	void serialPortCtrl_bytesWritten() {/*use syn mode*/}
	void serialPortCtrl_error(QSerialPort::SerialPortError error) { if (error > 0) QMessageBox::warning(this, "Warning", QString("Error occured and the error code: ") + aaa::num2string(error).c_str()); }
	
public://Write serialport
	typedef struct PortParams
	{
		CMDCODE cmdCode;
		uchar motorId; // 0x0 or 0x1 or 0x2 or 0x3
		int moveDirection; //-1 or 1
		int moveDistance;//diy
		int movePrecision;//diy
	}PortParams;
	static void writeSerialPortCtrl(PortParams pp, MyWidget *self)
	{
		//0.
		uchar data[12];
		data[0] = 0x53;
		data[10] = pp.cmdCode;
		data[11] = 0x0;

		//1.
		if (pp.cmdCode == CMD_SJ_GODIST)
		{	
			*((int*)(data + 3)) = pp.moveDirection * pp.moveDistance * 1000000;
			data[7] = pp.motorId;
			*((int*)(data + 8)) = pp.movePrecision * 1000000;
		}
		else if (pp.cmdCode == CMD_SJ_GO_STOP)
		{
			data[2] = pp.motorId;
		}
		else if (pp.cmdCode == CMD_DEVRST)
		{
		}

		//3.
		for (int i = 1; i < 11; ++i) data[11] += data[i];

		//4.
		int k;
		for (k = 0; k < 3; ++k)
		{
			for (int i = 0; i < 3; ++i)
			{
				self->serialPortCtrl.write((char*)data, 12);
				if (self->serialPortCtrl.waitForBytesWritten(100)) break;
				QTime t; t.start();//prevent blocking main thread event loop
				while (t.elapsed()< 200)  QApplication::processEvents();
			}
			if (pp.cmdCode == self->slaveCmdCode) break;
			QTime t; t.start();//prevent blocking main thread event loop
			while (t.elapsed()< 200)  QApplication::processEvents();
		}
		if (k == 4) cout << endl << "Fail to send cmd( " << pp.cmdCode << ") to slave or receive slave cmd" << endl;
	}
	void pushButtonMoveRight_pressed() { PortParams pp = { CMD_SJ_GODIST, 0x0, 1, 200, 5}; writeSerialPortCtrl(pp, this); }
	void pushButtonMoveLeft_pressed() { PortParams pp = { CMD_SJ_GODIST,0x0, -1, 200, 5}; writeSerialPortCtrl(pp, this); }
	void pushButtonMoveForward_pressed() { PortParams pp = { CMD_SJ_GODIST,0x1, 1, 200, 5 }; writeSerialPortCtrl(pp, this); }
	void pushButtonMoveBackward_pressed() { PortParams pp = { CMD_SJ_GODIST, 0x1, -1, 200, 5 }; writeSerialPortCtrl(pp, this); }
	void pushButtonMoveUp_pressed() { PortParams pp = { CMD_SJ_GODIST, 0x2, -1, 200, 5 }; writeSerialPortCtrl(pp, this); }
	void pushButtonMoveDown_pressed() { PortParams pp = { CMD_SJ_GODIST, 0x2, 1, 200, 5 }; writeSerialPortCtrl(pp, this); }
	void pushButtonRotateClockwise_pressed() { PortParams pp = { CMD_SJ_GODIST, 0x3, 1, 200, 5 }; writeSerialPortCtrl(pp, this); }
	void pushButtonRotateAntiClockwise_pressed() { PortParams pp = { CMD_SJ_GODIST, 0x3, -1, 200, 5}; writeSerialPortCtrl(pp, this); }

	void pushButtonMoveRight_released() 
	{
		//0.
		PortParams pp = { CMD_SJ_GO_STOP, 0x0};
		writeSerialPortCtrl(pp, this);

		//1.
		pp.motorId = 0x1;
		writeSerialPortCtrl(pp, this);

		//2.
		pp.motorId = 0x2;
		writeSerialPortCtrl(pp, this);

		//3.
		pp.motorId = 0x3;
		writeSerialPortCtrl(pp, this);
	}
	void pushButtonMoveLeft_released() { pushButtonMoveRight_released(); }
	void pushButtonMoveForward_released() { pushButtonMoveRight_released(); }
	void pushButtonMoveBackward_released() { pushButtonMoveRight_released(); }
	void pushButtonMoveUp_released() { pushButtonMoveRight_released(); }
	void pushButtonMoveDown_released() { pushButtonMoveRight_released(); }
	void pushButtonRotateClockwise_released() { pushButtonMoveRight_released(); }
	void pushButtonRotateAntiClockwise_released() { pushButtonMoveRight_released(); }
	
	void pushButtonReset_clicked() { PortParams pp = { CMD_SJ_GODIST }; writeSerialPortCtrl(pp, this); }
public://
	long timeid = -1;
	long scanid = -1;
	float lasttimeX = FLT_MAX;
	float lasttimeY = FLT_MAX;
	float lasttimeZ = FLT_MAX;
	float lasttimeR = FLT_MAX;
	bool enableSave = false;
	bool clickSave = false;
	void GetAndSaveAndShowProfiles()
	{
		int iRetValue = 0;//buffer

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
		if((enableSave && radioButtonXAxis->isChecked() && lasttimeX - realtimeX > comboBoxStep->currentText().toInt()) ||
			(enableSave && radioButtonYAxis->isChecked() && lasttimeY - realtimeY > comboBoxStep->currentText().toInt()) || 
			clickSave)
		{
			QSqlQuery query(db);
			query.prepare("insert into tb_scan_details (timeid, scanid, xdata, zdata) values (?, ?, ?, ?)");
			query.addBindValue(timeid);
			query.addBindValue(scanid++);
			query.addBindValue(QByteArray::fromRawData((char*)(&vdValueX[0]), (int)vdValueX.size() * sizeof(double)));
			query.addBindValue(QByteArray::fromRawData((char*)(&vdValueZ[0]), (int)vdValueZ.size() * sizeof(double)));
			query.exec();

			lasttimeX = realtimeX;
			lasttimeY = realtimeY;
			clickSave = false;
			static int n = 0;
			this->setStatusTip(QString("saved: ") + aaa::num2string(++n).c_str());
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
		pushButtonOpenDataPort->setEnabled(enable);
		pushButtonOpenCtrlPort->setEnabled(enable);
		pushButtonOpenEthernet->setEnabled(enable);
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
	void customPlotViewScan_mousePress(QMouseEvent *event) { clickSave = true; }

	void PushButtonStartup_clickded()
	{
		if (timerContinousScan->isActive())
		{
			//1.
			timerContinousScan->stop();
			EnableOrDisableControls(true);
			timeid = -1;
			scanid = -1;
			enableSave = false;
			clickSave = false;

			//2.
		}
		else
		{
			//1.
			timerContinousScan->start(100);
			EnableOrDisableControls(false);
			timeid = time(0);
			scanid = 0;
			enableSave = true;
			clickSave = true;

			//2.

			//3.
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



public://Use sqlite
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