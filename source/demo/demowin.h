#include "window/windowaaa.h"
#include "InterfaceLLT_2.h"
#include "qcustomplot.h"
#include "CommDef.h"
#pragma execution_character_set("utf-8")


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
			QGroupBox *groupBoxPortSetting = new QGroupBox("通信设置", widgetScan);
				QGridLayout *gridLayoutGroupBoxPortSetting = new QGridLayout(groupBoxPortSetting);//addin4
				QComboBox *comboBoxDataPort = new QComboBox(groupBoxPortSetting);
				QPushButton *pushButtonOpenDataPort = new QPushButton("打开", groupBoxPortSetting);
				QComboBox *comboBoxCtrlPort = new QComboBox(groupBoxPortSetting);
				QPushButton *pushButtonOpenCtrlPort = new QPushButton("打开", groupBoxPortSetting);
				QPushButton *pushButtonOpenEthernet = new QPushButton("打开扫描设备", groupBoxPortSetting);
				QList<QSerialPortInfo> listSerialPortInfo = QSerialPortInfo::availablePorts();//special
			QGroupBox *groupBoxCurrentPose = new QGroupBox("当前姿态", widgetScan);
				QGridLayout *gridLayoutGroupBoxCurrentPose = new QGridLayout(groupBoxCurrentPose);//addin5
				QLineEdit *lineEditXAxis = new QLineEdit("-1.000", groupBoxCurrentPose);
				QLineEdit *lineEditYAxis = new QLineEdit("-1.000", groupBoxCurrentPose);
				QLineEdit *lineEditZAxis = new QLineEdit("-1.000", groupBoxCurrentPose);
				QLineEdit *lineEditRotate = new QLineEdit("-1.000", groupBoxCurrentPose);
			QGroupBox *groupBoxPoseSetting = new QGroupBox("姿态设置", widgetScan);
				QGridLayout *gridLayoutPoseSetting = new QGridLayout(groupBoxPoseSetting);//addin6
				QPushButton *pushButtonMoveRight = new QPushButton("向右移", groupBoxPoseSetting);
				QPushButton *pushButtonMoveLeft = new QPushButton("向左移", groupBoxPoseSetting);
				QPushButton *pushButtonMoveForward = new QPushButton("向前移", groupBoxPoseSetting);
				QPushButton *pushButtonMoveBackward = new QPushButton("向后移", groupBoxPoseSetting);
				QPushButton *pushButtonMoveUp = new QPushButton("向上移", groupBoxPoseSetting);
				QPushButton *pushButtonMoveDown = new QPushButton("向下移", groupBoxPoseSetting);
				QPushButton *pushButtonRotateClockwise = new QPushButton("顺旋转", groupBoxPoseSetting);
				QPushButton *pushButtonRotateAntiClockwise = new QPushButton("逆旋转", groupBoxPoseSetting);
			QGroupBox *groupBoxScanSetting = new QGroupBox("扫描设置", widgetScan);
				QGridLayout *gridLayoutGroupBoxScanSetting = new QGridLayout(groupBoxScanSetting);//addin7
				QRadioButton *radioButtonXAxis = new QRadioButton("X轴", groupBoxScanSetting);
				QRadioButton *radioButtonYAxis = new QRadioButton("Y轴", groupBoxScanSetting);
				QDoubleSpinBox *doubleSpinBoxSpeed = new QDoubleSpinBox(groupBoxScanSetting);
				QSpinBox *spinBoxDistance = new QSpinBox(groupBoxScanSetting);
				QComboBox *comboBoxStep = new QComboBox(groupBoxScanSetting);
			QGroupBox *groupBoxImplement = new QGroupBox("执行扫描", widgetScan);
				QGridLayout *gridLayoutGroupBoxImplement = new QGridLayout(groupBoxImplement);//addin8
				QPushButton *pushButtonReset = new QPushButton("重置", groupBoxImplement);
				QPushButton *PushButtonStartup = new QPushButton("启动", groupBoxImplement);
		QWidget *widgetAnalyse = new QWidget(tabWidgetScanAnalyse);
			QGridLayout *gridLayoutWidgetAnalyse = new QGridLayout(widgetAnalyse);//addin9
			QTableView *tableViewCatalog = new QTableView(widgetAnalyse);
			QTableView *tableViewDetails = new QTableView(widgetAnalyse);
			QCustomPlot *customPlotViewAnlyse = new QCustomPlot(widgetAnalyse);
			QCPTextElement *cpTextElementXY = new QCPTextElement(customPlotViewAnlyse, "(-1.000,  -1.000)", QFont("", 15, QFont::Thin));
			QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "connect_name_of_sqlite");//addin10
			QSqlTableModel *tableModelCatalog = new QSqlTableModel(widgetAnalyse, db);
			QSqlQueryModel *queryModelDetails = new QSqlQueryModel(widgetAnalyse);		
#endif
	void createMainUI()
	{
		//0.
		this->setWindowTitle("断面扫描系统");
		this->setWindowIcon(QIcon("./../data/window/boss.ico"));
		this->setMinimumSize(QSize(800, 600));
		this->setFont(QFont("", 20, QFont::Thin));

		//addin1
		vboxLayoutWorkArea->addWidget(tabWidgetScanAnalyse);

		//addin2
		tabWidgetScanAnalyse->addTab(widgetScan, "断面扫描");
		tabWidgetScanAnalyse->addTab(widgetAnalyse, "历史记录"); widgetAnalyse->setFont(QFont("", 10, QFont::Thin));

		//addin3
		gridLayoutWidgetScan->addWidget(customPlotViewScan, 0, 0, 1, 4); customPlotViewScan->setInteractions(QCP::iRangeZoom | QCP::iSelectPlottables); customPlotViewScan->addGraph();
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
		gridLayoutGroupBoxPortSetting->addWidget(new QLabel("数据串口", groupBoxPortSetting), 0, 0, 1, 1);
		gridLayoutGroupBoxPortSetting->addWidget(comboBoxDataPort, 0, 1, 1, 1); for (int i = 0; i < listSerialPortInfo.size(); ++i) comboBoxDataPort->addItem(listSerialPortInfo[i].portName());
		gridLayoutGroupBoxPortSetting->addWidget(pushButtonOpenDataPort, 0, 2, 1, 1);
		gridLayoutGroupBoxPortSetting->addWidget(new QLabel("控制串口", groupBoxPortSetting), 0, 4, 1, 1);
		gridLayoutGroupBoxPortSetting->addWidget(comboBoxCtrlPort, 0, 5, 1, 1); for (int i = 0; i < listSerialPortInfo.size(); ++i) comboBoxCtrlPort->addItem(listSerialPortInfo[i].portName());
		gridLayoutGroupBoxPortSetting->addWidget(pushButtonOpenCtrlPort, 0, 6, 1, 1);
		gridLayoutGroupBoxPortSetting->addWidget(pushButtonOpenEthernet, 0, 8, 1, 1);
		gridLayoutGroupBoxPortSetting->setColumnStretch(0, 0);
		gridLayoutGroupBoxPortSetting->setColumnStretch(1, 1);
		gridLayoutGroupBoxPortSetting->setColumnStretch(2, 1);
		gridLayoutGroupBoxPortSetting->setColumnStretch(3, 1);
		gridLayoutGroupBoxPortSetting->setColumnStretch(4, 0);
		gridLayoutGroupBoxPortSetting->setColumnStretch(5, 1);
		gridLayoutGroupBoxPortSetting->setColumnStretch(6, 1);
		gridLayoutGroupBoxPortSetting->setColumnStretch(7, 1);
		gridLayoutGroupBoxPortSetting->setColumnStretch(8, 1);
		connect(pushButtonOpenDataPort, &QPushButton::clicked, this, &MyWidget::pushButtonOpenDataPort_clickded);
		connect(pushButtonOpenCtrlPort, &QPushButton::clicked, this, &MyWidget::pushButtonOpenCtrlPort_clickded);
		connect(pushButtonOpenEthernet, &QPushButton::clicked, this, &MyWidget::pushButtonOpenEthernet_clickded);

		//addin5
		gridLayoutGroupBoxCurrentPose->addWidget(new QLabel("X 坐标", groupBoxCurrentPose), 0, 0, 1, 1);
		gridLayoutGroupBoxCurrentPose->addWidget(lineEditXAxis, 0, 1, 1, 1); lineEditXAxis->setEnabled(false);
		gridLayoutGroupBoxCurrentPose->addWidget(new QLabel("毫米", groupBoxCurrentPose), 0, 2, 1, 1);
		gridLayoutGroupBoxCurrentPose->addWidget(new QLabel("Y 坐标", groupBoxCurrentPose), 1, 0, 1, 1);
		gridLayoutGroupBoxCurrentPose->addWidget(lineEditYAxis, 1, 1, 1, 1); lineEditYAxis->setEnabled(false);
		gridLayoutGroupBoxCurrentPose->addWidget(new QLabel("毫米", groupBoxCurrentPose), 1, 2, 1, 1);
		gridLayoutGroupBoxCurrentPose->addWidget(new QLabel("Z 坐标", groupBoxCurrentPose), 2, 0, 1, 1);
		gridLayoutGroupBoxCurrentPose->addWidget(lineEditZAxis, 2, 1, 1, 1); lineEditZAxis->setEnabled(false);
		gridLayoutGroupBoxCurrentPose->addWidget(new QLabel("毫米", groupBoxCurrentPose), 2, 2, 1, 1);
		gridLayoutGroupBoxCurrentPose->addWidget(new QLabel("俯仰角", groupBoxCurrentPose), 3, 0, 1, 1);
		gridLayoutGroupBoxCurrentPose->addWidget(lineEditRotate, 3, 1, 1, 1); lineEditRotate->setEnabled(false);
		gridLayoutGroupBoxCurrentPose->addWidget(new QLabel("角度", groupBoxCurrentPose), 3, 2, 1, 1);

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
		gridLayoutGroupBoxScanSetting->addWidget(new QLabel("扫描轴", groupBoxScanSetting), 0, 0, 1, 1);
		gridLayoutGroupBoxScanSetting->addWidget(radioButtonXAxis, 0, 1, 1, 1); radioButtonXAxis->setChecked(true); 
		gridLayoutGroupBoxScanSetting->addWidget(radioButtonYAxis, 0, 2, 1, 1); 
		gridLayoutGroupBoxScanSetting->addWidget(new QLabel("扫描速度", groupBoxScanSetting), 1, 0, 1, 1);
		gridLayoutGroupBoxScanSetting->addWidget(doubleSpinBoxSpeed, 1, 1, 1, 1); doubleSpinBoxSpeed->setMinimum(1.0); doubleSpinBoxSpeed->setMaximum(5.0); doubleSpinBoxSpeed->setValue(3.0);
		gridLayoutGroupBoxScanSetting->addWidget(new QLabel("毫米/秒", groupBoxScanSetting), 1, 2, 1, 1);
		gridLayoutGroupBoxScanSetting->addWidget(new QLabel("扫描长度", groupBoxScanSetting), 2, 0, 1, 1);
		gridLayoutGroupBoxScanSetting->addWidget(spinBoxDistance, 2, 1, 1, 2); spinBoxDistance->setMinimum(-100);
		gridLayoutGroupBoxScanSetting->addWidget(new QLabel("毫米", groupBoxScanSetting), 2, 3, 1, 1);
		gridLayoutGroupBoxScanSetting->addWidget(new QLabel("扫描步长", groupBoxScanSetting), 3, 0, 1, 1);
		gridLayoutGroupBoxScanSetting->addWidget(comboBoxStep, 3, 1, 1, 2); comboBoxStep->addItems(QStringList() << "2" << "5" << "10");
		gridLayoutGroupBoxScanSetting->addWidget(new QLabel("毫米", groupBoxScanSetting), 3, 3, 1, 1);

		//addin8
		gridLayoutGroupBoxImplement->addWidget(pushButtonReset, 0, 0, 1, 1); pushButtonReset->setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred));
		gridLayoutGroupBoxImplement->addWidget(PushButtonStartup, 1, 0, 1, 1); PushButtonStartup->setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred));
		connect(pushButtonReset, &QPushButton::clicked, this, &MyWidget::pushButtonReset_clicked);
		connect(PushButtonStartup, &QPushButton::clicked, this, &MyWidget::PushButtonStartup_clickded);

		//addin9
		gridLayoutWidgetAnalyse->addWidget(tableViewCatalog, 0, 0); tableViewCatalog->setSelectionBehavior(QAbstractItemView::SelectRows);
		gridLayoutWidgetAnalyse->addWidget(tableViewDetails, 0, 1); tableViewDetails->setSelectionBehavior(QAbstractItemView::SelectRows);
		gridLayoutWidgetAnalyse->addWidget(customPlotViewAnlyse, 1, 0, 1, 2); customPlotViewAnlyse->setInteractions(QCP::iRangeZoom | QCP::iSelectPlottables);
		customPlotViewAnlyse->addGraph(); 
		customPlotViewAnlyse->plotLayout()->insertRow(0); 
		customPlotViewAnlyse->plotLayout()->addElement(0, 0, cpTextElementXY);

		gridLayoutWidgetAnalyse->setRowStretch(0, 2);
		gridLayoutWidgetAnalyse->setRowStretch(1, 3);
		gridLayoutWidgetAnalyse->setColumnStretch(0, 3);
		gridLayoutWidgetAnalyse->setColumnStretch(1, 2);
		connect(tableViewCatalog, &QTableView::clicked, this, &MyWidget::tableViewCatalog_clicked);
		connect(tableViewDetails, &QTableView::clicked, this, &MyWidget::tableViewDetails_clicked);
		connect(customPlotViewAnlyse, &QCustomPlot::mousePress, this, &MyWidget::customPlotViewAnlyse_mousePress);

		//addin10
		db.setDatabaseName("./../data/mysqlite.db");
		if (!db.open()) { QMessageBox::information(this, "", "失败打开数据库, 错信息如下:\n" + db.lastError().text());  QApplication::exit(); }

		tableModelCatalog->setTable("tb_scan_catalog");
		tableModelCatalog->sort(0, Qt::DescendingOrder);
		tableModelCatalog->setEditStrategy(QSqlTableModel::OnManualSubmit);
		if (!tableModelCatalog->select()) { QMessageBox::information(this, "", tableModelCatalog->lastError().text()); return; }
		tableViewCatalog->setModel(tableModelCatalog);
		tableModelCatalog->setHeaderData(0, Qt::Horizontal, "扫描主键");
		tableModelCatalog->setHeaderData(1, Qt::Horizontal, "扫描方向");
		tableModelCatalog->setHeaderData(2, Qt::Horizontal, "扫描长度");
		tableModelCatalog->setHeaderData(3, Qt::Horizontal, "扫描步长");

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
			pushButtonOpenDataPort->setText("打开");
			disconnect(&serialPortXYZR, &QSerialPort::readyRead, this, &MyWidget::serialPortXYZR_readyRead);
			disconnect(&serialPortXYZR, &QSerialPort::bytesWritten, this, &MyWidget::serialPortXYZR_bytesWritten);
			disconnect(&serialPortXYZR, static_cast<void(QSerialPort::*)(QSerialPort::SerialPortError)>(&QSerialPort::error), this, &MyWidget::serialPortXYZR_error);
		}
		else
		{
			serialPortXYZR.setPort(QSerialPortInfo(comboBoxDataPort->currentText()));
			serialPortXYZR.setBaudRate(115200);
			if (serialPortXYZR.open(QIODevice::ReadWrite))
			{
				pushButtonOpenDataPort->setText("关闭");
				connect(&serialPortXYZR, &QSerialPort::readyRead, this, &MyWidget::serialPortXYZR_readyRead);
				connect(&serialPortXYZR, &QSerialPort::bytesWritten, this, &MyWidget::serialPortXYZR_bytesWritten);
				connect(&serialPortXYZR, static_cast<void(QSerialPort::*)(QSerialPort::SerialPortError)>(&QSerialPort::error), this, &MyWidget::serialPortXYZR_error);
			}
			else QMessageBox::information(this, "", serialPortXYZR.portName() + "打开失败");
		}
	}
	void pushButtonOpenCtrlPort_clickded() 
	{
		if (serialPortCtrl.isOpen())
		{
			serialPortCtrl.close();
			pushButtonOpenCtrlPort->setText("打开");
			disconnect(&serialPortCtrl, &QSerialPort::readyRead, this, &MyWidget::serialPortCtrl_readyRead);
			disconnect(&serialPortCtrl, &QSerialPort::bytesWritten, this, &MyWidget::serialPortCtrl_bytesWritten);
			disconnect(&serialPortCtrl, static_cast<void(QSerialPort::*)(QSerialPort::SerialPortError)>(&QSerialPort::error), this, &MyWidget::serialPortCtrl_error);
		}
		else
		{
			serialPortCtrl.setPort(QSerialPortInfo(comboBoxCtrlPort->currentText()));
			serialPortCtrl.setBaudRate(115200);
			if (serialPortCtrl.open(QIODevice::ReadWrite))
			{
				pushButtonOpenCtrlPort->setText("关闭");
				connect(&serialPortCtrl, &QSerialPort::readyRead, this, &MyWidget::serialPortCtrl_readyRead);
				connect(&serialPortCtrl, &QSerialPort::bytesWritten, this, &MyWidget::serialPortCtrl_bytesWritten);
				connect(&serialPortCtrl, static_cast<void(QSerialPort::*)(QSerialPort::SerialPortError)>(&QSerialPort::error), this, &MyWidget::serialPortCtrl_error);
			}
			else QMessageBox::information(this, "", serialPortCtrl.portName() + "打开失败");
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
				pushButtonOpenEthernet->setText("退出扫描系统");
				connect(timerContinousScan, &QTimer::timeout, this, &MyWidget::timerContinousScan_timeout); 
			}
			else QMessageBox::information(this, "", "打开扫描设备失败"); 
		}
		else QApplication::exit();
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
		realtimeR = ((int*)(data + 1))[3] / 1000.f;
		lineEditXAxis->setText(aaa::num2string(realtimeX, 3).c_str());
		lineEditYAxis->setText(aaa::num2string(realtimeY, 3).c_str());
		lineEditZAxis->setText(aaa::num2string(realtimeZ, 3).c_str());
		lineEditRotate->setText(aaa::num2string(realtimeR, 3).c_str());
	}
	void serialPortXYZR_bytesWritten() {/*no use*/}
	void serialPortXYZR_error(QSerialPort::SerialPortError error) { if (error > 0) QMessageBox::warning(this, "", QString("串口发生错误, 错误码为: ") + aaa::num2string(error).c_str()); }

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
	void serialPortCtrl_error(QSerialPort::SerialPortError error) { if (error > 0) QMessageBox::warning(this, "", QString("串口发生错误, 错误码为: ") + aaa::num2string(error).c_str()); }
	
public://Write serialport
	typedef struct PortParams
	{
		CMDCODE cmdCode;
		int motorId; // 0x0 or 0x1 or 0x2 or 0x3
		int moveDistance;//diy
		double moveSpeed;//diy
	}PortParams;
	static void writeSerialPortCtrl(PortParams pp, MyWidget *self)
	{
		//0.
		static int cmdid = 0;
		uchar data[12];
		data[0] = 0x53;
		data[1] = pp.cmdCode;
		data[10] = ++cmdid;
		data[11] = 0x0;

		//1.
		if (pp.cmdCode == CMD_SJ_GODIST)
		{	
			data[2] = pp.motorId;
			*((int*)(data + 3)) = pp.moveDistance * 1000000;
			*((short*)(data + 7)) = 0x1;
			*((short*)(data + 8)) = (short)(pp.moveSpeed * 1000);
		}
		else if (pp.cmdCode == CMD_SJ_GO_STOP)
		{
			data[2] = pp.motorId;
		}
		else if (pp.cmdCode == CMD_DEVRST)
		{
		}
		else if (pp.cmdCode == CMD_SJ_REPEAT_GO)
		{
			data[2] = pp.motorId;//0x0--start   0x1---stop
		}

		//3.
		for (int i = 1; i < 11; ++i) data[11] += data[i];

		//4.
		self->serialPortCtrl.write((char*)data, 12);
		bool send = self->serialPortCtrl.waitForBytesWritten(100);

		//5.
		bool receive = false;
		QTime t; t.start();
		while (t.elapsed() < 2000)
		{	
			QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
			if (pp.cmdCode == self->slaveCmdCode) { self->slaveCmdCode = CMD_INVALID; receive = true; break; }
		}

		//6.
		string tips = "id=" + aaa::num2string(cmdid) + " send=" + (send ? "1" : "0") +" receive=" + (receive ? "1" : "0");
		self->setWindowTitle((self->windowTitle().size() > 200 ? QString("断面扫描系统") : self->windowTitle()) + "     " + tips.c_str());
	}
	void pushButtonMoveRight_pressed() { if(!serialPortCtrl.isOpen()) return; PortParams pp = { CMD_SJ_GODIST, 0x0, 200, 5 }; writeSerialPortCtrl(pp, this); }
	void pushButtonMoveLeft_pressed() { if (!serialPortCtrl.isOpen()) return; PortParams pp = { CMD_SJ_GODIST,0x0, -200, 5 }; writeSerialPortCtrl(pp, this); }
	void pushButtonMoveForward_pressed() { if (!serialPortCtrl.isOpen()) return; PortParams pp = { CMD_SJ_GODIST,0x1, 200, 5 }; writeSerialPortCtrl(pp, this); }
	void pushButtonMoveBackward_pressed() { if (!serialPortCtrl.isOpen()) return; PortParams pp = { CMD_SJ_GODIST, 0x1, -200, 5 }; writeSerialPortCtrl(pp, this); }
	void pushButtonMoveUp_pressed() { if (!serialPortCtrl.isOpen()) return; PortParams pp = { CMD_SJ_GODIST, 0x2, 200, 5 }; writeSerialPortCtrl(pp, this); }
	void pushButtonMoveDown_pressed() { if (!serialPortCtrl.isOpen()) return; PortParams pp = { CMD_SJ_GODIST, 0x2, -200, 5 }; writeSerialPortCtrl(pp, this); }
	void pushButtonRotateClockwise_pressed() { if (!serialPortCtrl.isOpen()) return; PortParams pp = { CMD_SJ_GODIST, 0x3, -2, 0.06 }; writeSerialPortCtrl(pp, this); }
	void pushButtonRotateAntiClockwise_pressed() { if (!serialPortCtrl.isOpen()) return; PortParams pp = { CMD_SJ_GODIST, 0x3, 2, 0.06 }; writeSerialPortCtrl(pp, this); }

	void pushButtonMoveRight_released() { if (!serialPortCtrl.isOpen()) return; PortParams pp = { CMD_SJ_GO_STOP, 0x0}; writeSerialPortCtrl(pp, this);}
	void pushButtonMoveLeft_released() { if (!serialPortCtrl.isOpen()) return; PortParams pp = { CMD_SJ_GO_STOP, 0x0 }; writeSerialPortCtrl(pp, this); }
	void pushButtonMoveForward_released() { if (!serialPortCtrl.isOpen()) return; PortParams pp = { CMD_SJ_GO_STOP, 0x1 }; writeSerialPortCtrl(pp, this); }
	void pushButtonMoveBackward_released() { if (!serialPortCtrl.isOpen()) return; PortParams pp = { CMD_SJ_GO_STOP, 0x1 }; writeSerialPortCtrl(pp, this); }
	void pushButtonMoveUp_released() { if (!serialPortCtrl.isOpen()) return; PortParams pp = { CMD_SJ_GO_STOP, 0x2 }; writeSerialPortCtrl(pp, this); }
	void pushButtonMoveDown_released() { if (!serialPortCtrl.isOpen()) return; PortParams pp = { CMD_SJ_GO_STOP, 0x2 }; writeSerialPortCtrl(pp, this); }
	void pushButtonRotateClockwise_released() { if (!serialPortCtrl.isOpen()) return; PortParams pp = { CMD_SJ_GO_STOP, 0x3 }; writeSerialPortCtrl(pp, this); }
	void pushButtonRotateAntiClockwise_released() { if (!serialPortCtrl.isOpen()) return; PortParams pp = { CMD_SJ_GO_STOP, 0x3 }; writeSerialPortCtrl(pp, this); }
	
	void pushButtonReset_clicked() { if (!serialPortCtrl.isOpen()) return; PortParams pp = { CMD_DEVRST }; writeSerialPortCtrl(pp, this); }

public://
	long timeid = -1;
	long scanid = -1;
	bool enableSave = false;
	bool clickSave = false;
	float lasttimeX, initialX;
	float lasttimeY, initialY;
	float lasttimeZ, initialZ;
	float lasttimeR, initialR;
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
		if((enableSave && radioButtonXAxis->isChecked() && __abs(lasttimeX - realtimeX) > comboBoxStep->currentText().toInt()) ||
			(enableSave && radioButtonYAxis->isChecked() && __abs(lasttimeY - realtimeY) > comboBoxStep->currentText().toInt()) ||
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
			string tips = string("saved=") + aaa::num2string(scanid).c_str();
			this->setWindowTitle((this->windowTitle().size() > 200 ? QString("断面扫描系统") : this->windowTitle()) + "     " + tips.c_str());
		}

		//4.Stop scan
		if ((enableSave && radioButtonXAxis->isChecked() && __abs(realtimeX - initialX) > spinBoxDistance->value()) ||
			(enableSave && radioButtonYAxis->isChecked() && __abs(realtimeY - initialY) > spinBoxDistance->value())) startOrStopScan(true);

		//5.Show profiles
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
		PushButtonStartup->setText(enable ? "启动" : "终止");
	}
	void customPlotViewScan_mousePress(QMouseEvent *event) { /*clickSave = true;*/ }

public:
	void startOrStopScan(bool stop)
	{
		if (stop)
		{
			//1.
			timerContinousScan->stop();
			EnableOrDisableControls(true);
			timeid = -1;
			scanid = -1;
			enableSave = false;
			clickSave = false;

			//2.
			PortParams pp = { CMD_SJ_GO_STOP, radioButtonXAxis->isChecked() ? 0x0 : 0x1 };
			writeSerialPortCtrl(pp, this);
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
			lasttimeX = initialX = realtimeX;
			lasttimeY = initialY = realtimeY;
			lasttimeZ = initialZ = realtimeZ;
			lasttimeR = initialR = realtimeR;

			//2.
			PortParams pp = { CMD_SJ_GODIST, radioButtonXAxis->isChecked() ? 0x0 : 0x1, spinBoxDistance->value(), doubleSpinBoxSpeed->value() };
			writeSerialPortCtrl(pp, this);

			//3.
			QSqlQuery query(db);
			query.prepare("insert into tb_scan_catalog (timeid, direction, distance, step) values (?, ?, ?, ?)");
			query.addBindValue(timeid);
			query.addBindValue(radioButtonXAxis->isChecked() ? "X" : "Y");
			query.addBindValue(spinBoxDistance->value());
			query.addBindValue(comboBoxStep->currentText().toInt());
			query.exec();
		}
	}
	void PushButtonStartup_clickded()
	{
		if (!serialPortCtrl.isOpen()) return;
		if (!m_pLLT) return;
		startOrStopScan(timerContinousScan->isActive());
	}

public://Use sqlite
	void tableViewCatalog_clicked(QModelIndex modelIndex)
	{
		QSqlRecord record = tableModelCatalog->record(modelIndex.row());
		queryModelDetails->setQuery("select * from tb_scan_details where timeid=" + record.value(0).toString(), db);

		tableViewDetails->hideColumn(0);
		queryModelDetails->setHeaderData(1, Qt::Horizontal, "扫描序号");
		tableViewDetails->hideColumn(2);
		tableViewDetails->hideColumn(3);
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
	void customPlotViewAnlyse_mousePress(QMouseEvent* event)
	{
		string str("(");
		str += aaa::num2string(customPlotViewAnlyse->xAxis->pixelToCoord(event->pos().x()), 3);
		str += ",  ";
		str += aaa::num2string(customPlotViewAnlyse->yAxis->pixelToCoord(event->pos().y()), 3);
		str += ")";
		cpTextElementXY->setText(str.c_str());
	}
};


#endif