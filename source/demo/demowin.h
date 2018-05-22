#include "window/windowaaa.h"
#include "InterfaceLLT_2.h"
#include "qcustomplot.h"
#include "CommDef.h"
#pragma execution_character_set("utf-8")
#define USE_SIMULATION 1
int timeout_for_simulation = 500;

#ifndef __demowin_h__
#define __demowin_h__

class DemoCQMeasurement : public QDialog
{
public://User events

public://Interruption events

public://DIY code

public://DIY UI

public://UI members
	QGridLayout *gridLayoutWidgetMain = new QGridLayout(this);
	QChartView *chartView = new QChartView(this);
	QPushButton *pushButtonSlotAnalysis = new QPushButton("激活槽宽槽深计算功能", this);
	QLineEdit *lineEidtSlotPoint1 = new QLineEdit("", this);
	QLineEdit *lineEidtSlotPoint2 = new QLineEdit("", this);
	QLineEdit *lineEidtSlotWidth = new QLineEdit("", this);
	QLineEdit *lineEidtSlotHeight = new QLineEdit("", this);
	QPushButton *pushButtonSurfaceAnalysis = new QPushButton("激活曲面面差计算功能", this);
	QLineEdit *lineEidtSurfacePoint1 = new QLineEdit("", this);
	QLineEdit *lineEidtSurfacePoint2 = new QLineEdit("", this);
	QLineEdit *lineEidtSurfacePoint3 = new QLineEdit("", this);
	QLineEdit *lineEidtSurfacePoint4 = new QLineEdit("", this);
	QLineEdit *lineEidtSurfaceDiff = new QLineEdit("", this);

public://Data members
	QChart *chart = new QChart();//The API tells QtChart belongs to QtChartView, but QtChartView is also deleted when QtChart is deleted.

public://Init UI and Data
	DemoCQMeasurement(QWidget *parent = 0, QScatterSeries *series = 0) : QDialog(parent)
	{
		//0.Basic settting
		this->setWindowTitle("缝隙分析");
		this->setWindowIcon(QIcon("./../data/window/boss.ico"));
		this->setMinimumSize(QSize(800, 400));
		{
			chart->addSeries(series);
			//series->setMarkerShape(QScatterSeries::MarkerShapeCircle);
			series->setMarkerSize(5.0);
			//series->setPointsVisible(true);
			//chart->legend()->setVisible(false);
			chart->createDefaultAxes();
			chartView->setChart(chart);	
		}

		//1.Group1 setting
		gridLayoutWidgetMain->addWidget(chartView, 0, 0, 13, 1);
		gridLayoutWidgetMain->addWidget(pushButtonSlotAnalysis, 0, 1, 1, 2);
		gridLayoutWidgetMain->addWidget(new QLabel("第一点", this), 1, 1, 1, 1);
		gridLayoutWidgetMain->addWidget(lineEidtSlotPoint1, 1, 2, 1, 1);
		gridLayoutWidgetMain->addWidget(new QLabel("第二点", this), 2, 1, 1, 1);
		gridLayoutWidgetMain->addWidget(lineEidtSlotPoint2, 2, 2, 1, 1);
		gridLayoutWidgetMain->addWidget(new QLabel("槽  宽", this), 3, 1, 1, 1);
		gridLayoutWidgetMain->addWidget(lineEidtSlotWidth, 3, 2, 1, 1);
		gridLayoutWidgetMain->addWidget(new QLabel("槽  深", this), 4, 1, 1, 1);
		gridLayoutWidgetMain->addWidget(lineEidtSlotHeight, 4, 2, 1, 1);
		gridLayoutWidgetMain->addWidget(pushButtonSurfaceAnalysis, 7, 1, 1, 2);
		gridLayoutWidgetMain->addWidget(new QLabel("第一点", this), 8, 1, 1, 1);
		gridLayoutWidgetMain->addWidget(lineEidtSurfacePoint1, 8, 2, 1, 1);
		gridLayoutWidgetMain->addWidget(new QLabel("第二点", this), 9, 1, 1, 1);
		gridLayoutWidgetMain->addWidget(lineEidtSurfacePoint2, 9, 2, 1, 1);
		gridLayoutWidgetMain->addWidget(new QLabel("第三点", this), 10, 1, 1, 1);
		gridLayoutWidgetMain->addWidget(lineEidtSurfacePoint3, 10, 2, 1, 1);
		gridLayoutWidgetMain->addWidget(new QLabel("第四点", this), 11, 1, 1, 1);
		gridLayoutWidgetMain->addWidget(lineEidtSurfacePoint4, 11, 2, 1, 1);
		gridLayoutWidgetMain->addWidget(new QLabel("面  差", this), 12, 1, 1, 1);
		gridLayoutWidgetMain->addWidget(lineEidtSurfaceDiff, 12, 2, 1, 1);
		gridLayoutWidgetMain->setColumnStretch(0, 1);
		gridLayoutWidgetMain->setColumnStretch(1, 0);
		gridLayoutWidgetMain->setColumnStretch(1, 0);
	}
};

class DemoCQScan : public QWidget
{
public://User events
	void pushButtonOpenDataPort_clickded()
	{
		if (portXYZR.isOpen()) 
		{ 
			portXYZR.close(); 
			pushButtonOpenDataPort->setText("打开");
			this->setWindowTitle(portXYZR.portName() + " closed with success");
		}
		else
		{
			portXYZR.setPort(QSerialPortInfo(comboBoxDataPort->currentText()));
			portXYZR.setBaudRate(115200);
			if (portXYZR.open(QIODevice::ReadWrite))
			{
				pushButtonOpenDataPort->setText("关闭");
				this->setWindowTitle(portXYZR.portName() + " openned with success");
			}
			else
			{
				this->setWindowTitle(portXYZR.portName() + " openned with failure");
				QMessageBox::information(this, "", portXYZR.portName() + " openned with failure");
			}
		}
	}
	void pushButtonOpenCtrlPort_clickded()
	{
		if (portCtrl.isOpen())
		{ 
			portCtrl.close(); 
			pushButtonOpenCtrlPort->setText("打开"); 
			this->setWindowTitle(portCtrl.portName() + " closed with success");
		}
		else
		{
			portCtrl.setPort(QSerialPortInfo(comboBoxCtrlPort->currentText()));
			portCtrl.setBaudRate(115200);
			if (portCtrl.open(QIODevice::ReadWrite))
			{
				pushButtonOpenCtrlPort->setText("关闭");
				this->setWindowTitle(portCtrl.portName() + " openned with success");
			}
			else
			{
				this->setWindowTitle(portCtrl.portName() + " openned with failure");
				QMessageBox::information(this, "", portCtrl.portName() + " openned with failure");
			}
		}
	}
	void pushButtonOpenEthernet_clickded()
	{
		if (pushButtonOpenEthernet->text() == "退出扫描系统") QApplication::exit();

#if USE_SIMULATION
		m_pLLT = (CInterfaceLLT*)1;
		if (1) { this->setWindowTitle("Scan device openned with success"); timerContinousScan->start(timeout_for_simulation); scanMode = 0; }
#else
		if (initScanDevice()) { this->setWindowTitle("Scan device openned with success"); timerContinousScan->start(timeout_for_simulation); scanMode = 0; }
		else { this->setWindowTitle("Scan device openned with failure"); QMessageBox::information(this, "", "Scan device openned with failure"); }
#endif

		pushButtonOpenEthernet->setText("退出扫描系统");
	}
	bool initScanDevice(uint uiShutterTime = 100, uint uiIdleTime = 900)
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

	void motionCtrl_event(CMDCODE cmdCode, int motorId, int moveDistance, double moveSpeed)
	{
		if (!portCtrl.isOpen()) { this->setWindowTitle("portCtrl not opened"); QMessageBox::information(this, "", "portCtrl not opened"); return; }

		int iwrite = 1;
		//motionCtrol_unlock(false);
		static int cmdid = 0;
		portCtrlParams.cmdSended[0] = portCtrlParams.cmdHeader;
		portCtrlParams.cmdSended[1] = cmdCode;
		portCtrlParams.cmdSended[10] = ++cmdid;
		portCtrlParams.cmdSended[11] = 0x0;
		if (cmdCode == CMD_SJ_GODIST)
		{
			portCtrlParams.cmdSended[2] = motorId;
			*((int*)(portCtrlParams.cmdSended + 3)) = moveDistance * 1000000;
			*((short*)(portCtrlParams.cmdSended + 7)) = 0x1;
			*((short*)(portCtrlParams.cmdSended + 8)) = (short)(moveSpeed * 1000);
		}
		else if (cmdCode == CMD_SJ_GO_STOP) portCtrlParams.cmdSended[2] = motorId;
		else if (cmdCode == CMD_SJ_REPEAT_GO) portCtrlParams.cmdSended[2] = motorId;//0x0--start   0x1---stop
		for (int i = 1; i < 11; ++i) portCtrlParams.cmdSended[11] += portCtrlParams.cmdSended[i];
		portCtrlParams.hasSendSuccess = false;
		while (1)
		{
			//1.CameraThread: use 1 threads
			atomic_uchar runState = 0xFE; llong startTime = aaa_ns; llong outTime = 2000000000;
			portCtrl.write((char*)portCtrlParams.cmdSended, 12);
			while (1)
			{
				if (runState == 0xFF) { this->setWindowTitle(QString("Write ") + aaa::num2string(iwrite).c_str() + " time and receive the reply"); /*motionCtrol_unlock(true);*/ return; }
				else if (aaa_ns - startTime < outTime) this->setWindowTitle(QString("Writing ") + aaa::num2string(iwrite).c_str() + " time");
				else break;
				QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
				if (portCtrlParams.hasSendSuccess == true) runState = 0xFF;
			}
			if (++iwrite >= 3) { this->setWindowTitle("Write 3 times and receive no reply"); /*motionCtrol_unlock(true);*/ return; }
		}
	}

	void pushButtonSample_clicked()
	{
		if (!portCtrl.isOpen() || !portXYZR.isOpen() || !m_pLLT) { this->setWindowTitle("portCtrl or portXYZR or portScan not opened"); QMessageBox::information(this, "", "portCtrl or XYZR or portScan not opened"); return; }

		pushButtonSample->setEnabled(false);
		timeid = time(0);
		QSqlQuery query(db);
		query.prepare("insert into tb_scan_catalog (timeid, direction, velocity, distance, step) values (?, ?, ?, ?, ?)");
		query.addBindValue(timeid);
		query.addBindValue(comboBoxScanAxis->currentIndex() == 0 ? "X" : "Y");
		query.addBindValue(doubleSpinBoxScanSpeed->value());
		query.addBindValue(spinBoxScanDistance->value());
		query.addBindValue(comboBoxScanStep->currentText().toInt());
		query.exec();

		timerContinousScan->start(timeout_for_simulation); 
		scanMode = 1;
		while (1)
		{
			if (scanMode == 0) { pushButtonSample->setEnabled(true); break; }
			QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
		}	
	}
	void pushButtonAutomatic_clicked()
	{
		if (!portCtrl.isOpen() || !portXYZR.isOpen() || !m_pLLT) { this->setWindowTitle("portCtrl or portXYZR or portScan not opened"); QMessageBox::information(this, "", "portCtrl or XYZR or portScan not opened"); return; }

		pushButtonAutomatic->setEnabled(false);
		timeid = time(0);
		scanid = 0;
#if USE_SIMULATION
		lasttimeX = initialX = realtimeX = 0;
		lasttimeY = initialY = realtimeY = 0;
		lasttimeZ = initialZ = realtimeZ = 0;
		lasttimeR = initialR = realtimeR = 0;
#else 
		lasttimeX = initialX = realtimeX;
		lasttimeY = initialY = realtimeY;
		lasttimeZ = initialZ = realtimeZ;
		lasttimeR = initialR = realtimeR;
#endif
		QSqlQuery query(db);
		query.prepare("insert into tb_scan_catalog (timeid, direction, velocity, distance, step) values (?, ?, ?, ?, ?)");
		query.addBindValue(timeid);
		query.addBindValue(comboBoxScanAxis->currentIndex() == 0 ? "X" : "Y");
		query.addBindValue(doubleSpinBoxScanSpeed->value());
		query.addBindValue(spinBoxScanDistance->value());
		query.addBindValue(comboBoxScanStep->currentText().toInt());
		query.exec();

		motionCtrl_event(CMD_SJ_GODIST, comboBoxScanAxis->currentIndex() == 0 ? 0x0 : 0x1, spinBoxScanDistance->value(), doubleSpinBoxScanSpeed->value());

		scatter3D->seriesList().at(0)->dataProxy()->resetArray(0);//chart->removeAllSeries();
		timerContinousScan->start(timeout_for_simulation);
		scanMode = 2;
		while (1)
		{
			if (scanMode == 0) 
			{ 
				pushButtonAutomatic->setEnabled(true);
				timerContinousScan->stop(); 
				motionCtrl_event(CMD_SJ_GO_STOP, comboBoxScanAxis->currentIndex() == 0 ? 0x0 : 0x1, 0xFF, 0xFF);
				break; 
			}
			QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
		}
	}

	void tabWidget_tabBarClicked(int index)
	{
		if (index == 0)
		{
			scatter3D->seriesList().at(0)->dataProxy()->resetArray(0); if (!m_pLLT) { this->setWindowTitle("portScan not opened"); QMessageBox::information(this, "", "portScan not opened"); return; }
			if (timerContinousScan->isActive() == false) { timerContinousScan->start(timeout_for_simulation); scanMode = 0; }
		}
		else if (index == 1)
		{
			scatter3D->seriesList().at(0)->dataProxy()->resetArray(0);
			if (timerContinousScan->isActive()) { timerContinousScan->stop(); scanMode = 0; }
		}
		else if (index == 2)
		{
			scatter3D->seriesList().at(0)->dataProxy()->resetArray(0);
			if (timerContinousScan->isActive()) { timerContinousScan->stop(); scanMode = 0; }
			if (!tableModelCatalog->select()) { QMessageBox::information(this, "", tableModelCatalog->lastError().text()); return; }
		}
	}
	void tableViewCatalog_clicked(QModelIndex modelIndex)
	{
		QSqlRecord record = tableModelCatalog->record(modelIndex.row());
		queryModelDetails->setQuery("select * from tb_scan_details where timeid=" + record.value(0).toString(), db);

		tableViewDetails->hideColumn(0);
		queryModelDetails->setHeaderData(1, Qt::Horizontal, "扫描序号");
		queryModelDetails->setHeaderData(2, Qt::Horizontal, "扫描位置");
		queryModelDetails->setHeaderData(3, Qt::Horizontal, "扫描角度");
		tableViewDetails->hideColumn(4);
		tableViewDetails->hideColumn(5);
	}
	void tableViewDetails_clicked(QModelIndex modelIndex)
	{
		QSqlRecord record = queryModelDetails->record(modelIndex.row());

		double pos = record.value(2).toDouble();
		int sz = record.value(4).toByteArray().size() / sizeof(double);
		const double *xdata = (double*)(record.value(4).toByteArray().constData());//toByteArray is temperary, and data() is invalid
		const double *zdata = (double*)(record.value(5).toByteArray().constData());//but constData() is valid, don't know the reason.
		
		scatter3D->seriesList().at(0)->dataProxy()->resetArray(0);
		for(int i = 0; i < sz; ++i) scatter3D->seriesList().at(0)->dataProxy()->addItem(QScatterDataItem(QVector3D(xdata[i], zdata[i], pos)));
	}
	void tableViewDetails_doubleClicked(QModelIndex modelIndex)
	{
		QSqlRecord record = queryModelDetails->record(modelIndex.row());

		double pos = record.value(2).toDouble();
		int sz = record.value(4).toByteArray().size() / sizeof(double);
		const double *xdata = (double*)(record.value(4).toByteArray().constData());//toByteArray is temperary, and data() is invalid
		const double *zdata = (double*)(record.value(5).toByteArray().constData());//but constData() is valid, don't know the reason.

		QScatterSeries scatterSeries;
		for (int i = 0; i < sz; ++i) scatterSeries.append(xdata[i], zdata[i]);
		DemoCQMeasurement demoCQMeasurement(0, &scatterSeries);
		demoCQMeasurement.exec();
	}

public://Interruption events
	void portXYZR_readyRead()
	{
		//1.Readouta ll
		static queue<uchar> queueXYZR;
		QByteArray cmd = portXYZR.readAll();
		for (int i = 0; i < cmd.size(); ++i) queueXYZR.push((uchar)cmd[i]);

		//2.Its following data is useless because of no command header
		while (queueXYZR.size() > 0 && queueXYZR.front() != 0x57) queueXYZR.pop();

		//3.It cannot be decoded because of intact command
		if (queueXYZR.size() < 20) return;

		//4.Extract and decode command
		uchar data[18];
		uchar checksum = (uchar)0;
		queueXYZR.pop();
		for (int i = 0; i < 18; ++i)
		{
			data[i] = queueXYZR.front();
			queueXYZR.pop();
			checksum += data[i];
		}
		uchar trueChecksum = queueXYZR.front();
		queueXYZR.pop();
		if (checksum != trueChecksum) return;

		//6.Use command
		realtimeX = ((int*)(data + 1))[0] / 1000000.f;
		realtimeY = ((int*)(data + 1))[1] / 1000000.f;
		realtimeZ = ((int*)(data + 1))[2] / 1000000.f;
		realtimeR = ((int*)(data + 1))[3] / 1000.f;
		lineEditXAxis->setText(aaa::num2string(realtimeX, 3).c_str());
		lineEditYAxis->setText(aaa::num2string(realtimeY, 3).c_str());
		lineEditZAxis->setText(aaa::num2string(realtimeZ, 3).c_str());
		lineEditRotate->setText(aaa::num2string(realtimeR, 3).c_str());
	}
	void portCtrl_readyRead()
	{
		//1.Readout all
		static queue<uchar> queueCtrl;
		QByteArray cmd = portCtrl.readAll();
		for (int i = 0; i < cmd.size(); ++i) queueCtrl.push((uchar)cmd[i]);

		//2.Its following data is useless because of no command header
		while (queueCtrl.size() > 0 && queueCtrl.front() != 0x53) queueCtrl.pop();

		//3.It cannot be decoded because of intact command
		if (queueCtrl.size() < 12) return;

		//4.Extract and decode command
		uchar data[10];
		uchar checksum = (uchar)0;
		queueCtrl.pop();
		for (int i = 0; i < 10; ++i)
		{
			data[i] = queueCtrl.front();
			queueCtrl.pop();
			checksum += data[i];
		}
		uchar trueChecksum = queueCtrl.front();
		queueCtrl.pop();
		if (checksum == trueChecksum && data[0] == portCtrlParams.cmdSended[1]) portCtrlParams.hasSendSuccess = true;
	}
	void timerContinousScan_timeout()
	{
#if USE_SIMULATION
		realtimeX += 0.5; realtimeY += 0.5; realtimeZ += 0.5; realtimeR += 0.5;
		vector<double> vdValueX(1024);
		vector<double> vdValueZ(1024); int ratio = (rand() % (9 - 2 + 1) + 2); static int nn = 0; ++nn;
		if (nn % 3 == 0)
		{
			for (int i = 0; i < 1024; ++i)
			{
				vdValueX[i] = -30 + i * 0.05859375;
				vdValueZ[i] = vdValueX[i] * vdValueX[i] / 100 * ratio + 10;
			}
		}
		else if (nn % 3 == 1)
		{
			for (int i = 0; i < 1024; ++i)
			{
				vdValueX[i] = -30 + i * 0.05859375;
				vdValueZ[i] = vdValueX[i] * 20 / 100 * ratio + 10;
			}
		}
		else
		{
			for (int i = 0; i < 1024; ++i)
			{
				vdValueX[i] = -30 + i * 0.05859375;
				vdValueZ[i] = vdValueX[i] * vdValueX[i] * vdValueX[i] / 1000 * ratio + 10;
			}
		}
#else
		//0.Define variables
		int iRetValue = 0;
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
#endif
		//3.Scan mode: display or sample mode
		if (scanMode == 0 || scanMode == 1)
		{
			scatter3D->seriesList().at(0)->dataProxy()->resetArray(0);
			for (uint i = 0; i < vdValueX.size(); ++i) scatter3D->seriesList().at(0)->dataProxy()->addItem(QScatterDataItem(QVector3D(vdValueX[i], vdValueZ[i], 0)));

			//Display mode
			if (scanMode == 0) return;
			
			//Sample mode
			QSqlQuery query(db);
			query.prepare("insert into tb_scan_details (timeid, scanid, scanpos, scanang, xdata, zdata) values (?, ?, ?, ?, ?, ?)");
			query.addBindValue(timeid);
			query.addBindValue(0);//only do once
			query.addBindValue(comboBoxScanAxis->currentIndex() == 0 ? realtimeX : realtimeY);
			query.addBindValue(realtimeR);
			query.addBindValue(QByteArray::fromRawData((char*)(&vdValueX[0]), (int)vdValueX.size() * sizeof(double)));
			query.addBindValue(QByteArray::fromRawData((char*)(&vdValueZ[0]), (int)vdValueZ.size() * sizeof(double)));
			query.exec();

			scanMode = 0;//convert to display mode
			static int s = 0;
			this->setWindowTitle(QString("Sample with success: ") + aaa::num2string(++s).c_str());
			return;
		}

		//4.Scan mode: acquisition mode
		if ((comboBoxScanAxis->currentIndex() == 0 && __abs(lasttimeX - realtimeX) > comboBoxScanStep->currentText().toInt()) || (comboBoxScanAxis->currentIndex() == 1 && __abs(lasttimeY - realtimeY) > comboBoxScanStep->currentText().toInt()))
		{
			static int z = -15; z += 1; if (z > 15) { scanMode = 0; z = 0; }//less than 30 * 1024 points
			for (uint i = 0; i < vdValueX.size(); ++i) scatter3D->seriesList().at(0)->dataProxy()->addItem(QScatterDataItem(QVector3D(vdValueX[i], vdValueZ[i], z)));

			QSqlQuery query(db);
			query.prepare("insert into tb_scan_details (timeid, scanid, scanpos, scanang, xdata, zdata) values (?, ?, ?, ?, ?, ?)");
			query.addBindValue(timeid);
			query.addBindValue(scanid++);
			query.addBindValue(comboBoxScanAxis->currentIndex() == 0 ? realtimeX : realtimeY);
			query.addBindValue(realtimeR);
			query.addBindValue(QByteArray::fromRawData((char*)(&vdValueX[0]), (int)vdValueX.size() * sizeof(double)));
			query.addBindValue(QByteArray::fromRawData((char*)(&vdValueZ[0]), (int)vdValueZ.size() * sizeof(double)));
			query.exec();

			lasttimeX = realtimeX;
			lasttimeY = realtimeY;
			lasttimeR = realtimeR;
			this->setWindowTitle(QString("saved=") + aaa::num2string(scanid).c_str());
		}

		if ((comboBoxScanAxis->currentIndex() == 0 && __abs(realtimeX - initialX) > __abs(spinBoxScanDistance->value())) ||
			(comboBoxScanAxis->currentIndex() == 1 && __abs(realtimeY - initialY) > __abs(spinBoxScanDistance->value()))) scanMode = 0;
	}

public://DIY code
	typedef struct PortCtrlParams
	{
		//Interactive params
		atomic_bool hasExitThread; //For thread that cannot exit automatically
		atomic_bool hasSendSuccess;//For SerialPortMaster

		//Input Params
		uchar cmdHeader = 0x53;
		uchar cmdSended[256];//cmdHeader cmdLength ... checksum
		uchar cmdReceiced[256];//not need mutex (refer to timerMain comment)

		//Output Params

		//Buffer params	

		PortCtrlParams() { hasExitThread = false; hasSendSuccess = false; }
	}PortCtrlParams; PortCtrlParams portCtrlParams;
	
public://DIY UI

public://UI members

public://Data members
	QChart *chartMain = new QChart();//The API tells QtChart belongs to QtChartView, but QtChartView is also deleted when QtChart is deleted.

public://Init UI and Data
	DemoCQScan(QWidget *parent = 0) : QWidget(parent)
	{
		//0.Basic settting
		this->setWindowTitle("重庆市计量质量检测研究院缝隙检测仪");
		this->setWindowIcon(QIcon("./../data/window/boss.ico"));
		this->setMinimumSize(QSize(800, 600));
		this->setFont(QFont("", 20, QFont::Thin));
		{
			connect(&portXYZR, &QSerialPort::readyRead, this, &DemoCQScan::portXYZR_readyRead);
			connect(&portCtrl, &QSerialPort::readyRead, this, &DemoCQScan::portCtrl_readyRead);
			connect(timerContinousScan, &QTimer::timeout, this, &DemoCQScan::timerContinousScan_timeout);

			widget3D = QWidget::createWindowContainer(scatter3D, parent);
			scatter3D->addSeries(new QScatter3DSeries());
			scatter3D->seriesList().at(0)->setItemSize(0.03);
			scatter3D->axisX()->setTitle("Abscissa"); scatter3D->axisX()->setTitleVisible(true); scatter3D->axisX()->setLabelFormat("%.2f mm");
			scatter3D->axisY()->setTitle("Distance"); scatter3D->axisY()->setTitleVisible(true); scatter3D->axisY()->setLabelFormat("%.2f mm");
			scatter3D->axisZ()->setTitle("Ordinate"); scatter3D->axisZ()->setTitleVisible(true); scatter3D->axisZ()->setLabelFormat("%.2f mm");

			db.setDatabaseName("./../data/mysqlite.db");
			if (!db.open()) { QMessageBox::information(this, "", "失败打开数据库, 错信息如下:\n" + db.lastError().text());  QApplication::exit(); }
		}

		//1.Group1 setting  ---  MainWindow layout
		gridLayoutWidgetMain->addWidget(widget3D, 0, 0, 1, 1);
		gridLayoutWidgetMain->addWidget(tabWidget, 1, 0, 1, 1);
		tabWidget->addTab(widgetScanInitial, "采样设置");
		tabWidget->addTab(widgetScanAutomatic, "自动扫描"); 
		tabWidget->addTab(widgetScanHistroty, "历史记录");
		{	
			widgetScanHistroty->setFont(QFont("", 10, QFont::Thin));
			connect(tabWidget, &QTabWidget::tabBarClicked, this, &DemoCQScan::tabWidget_tabBarClicked);
		}
		gridLayoutWidgetMain->setRowStretch(0, 1);
		gridLayoutWidgetMain->setRowStretch(1, 0);

		//2.Group2 setting  ---  SampleScan layout
		gridLayoutScanInitial->addWidget(groupBoxPortSetting, 0, 0, 1, 1);
		gridLayoutScanInitial->addWidget(groupBoxCurrentPose, 0, 1, 1, 1);
		gridLayoutScanInitial->addWidget(groupBoxPoseSetting, 0, 2, 1, 1);
		gridLayoutScanInitial->addWidget(groupBoxSampleScan, 0, 3, 1, 1);
		gridLayoutScanInitial->setColumnStretch(0, 0);
		gridLayoutScanInitial->setColumnStretch(1, 2);
		gridLayoutScanInitial->setColumnStretch(2, 2);
		gridLayoutScanInitial->setColumnStretch(3, 1);

		//3.Group3 setting  ---  AutoScan layout
		gridLayoutScanAutomatic->addWidget(new QLabel("扫描轴", comboBoxScanAxis), 0, 0, 1, 1);
		gridLayoutScanAutomatic->addWidget(comboBoxScanAxis, 0, 1, 1, 1);
		gridLayoutScanAutomatic->addWidget(new QLabel("扫描速度", widgetScanAutomatic), 1, 0, 1, 1);
		gridLayoutScanAutomatic->addWidget(doubleSpinBoxScanSpeed, 1, 1, 1, 1);
		gridLayoutScanAutomatic->addWidget(new QLabel("扫描距离", widgetScanAutomatic), 2, 0, 1, 1);
		gridLayoutScanAutomatic->addWidget(spinBoxScanDistance, 2, 1, 1, 1);
		gridLayoutScanAutomatic->addWidget(new QLabel("扫描步长", widgetScanAutomatic), 3, 0, 1, 1);
		gridLayoutScanAutomatic->addWidget(comboBoxScanStep, 3, 1, 1, 1);
		gridLayoutScanAutomatic->addWidget(pushButtonAutomatic, 0, 2, 4, 1);
		{
			comboBoxScanAxis->addItems(QStringList() << "X 轴" << "Y 轴");
			doubleSpinBoxScanSpeed->setMinimum(1.0); 
			doubleSpinBoxScanSpeed->setMaximum(5.0); 
			doubleSpinBoxScanSpeed->setValue(3.0);
			spinBoxScanDistance->setMinimum(-100);
			comboBoxScanStep->addItems(QStringList() << "2" << "5" << "10");
			pushButtonAutomatic->setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred));
			connect(pushButtonAutomatic, &QPushButton::clicked, this, &DemoCQScan::pushButtonAutomatic_clicked);
		}
		gridLayoutScanAutomatic->setColumnStretch(0, 0);
		gridLayoutScanAutomatic->setColumnStretch(1, 1);
		gridLayoutScanAutomatic->setColumnStretch(2, 1);

		//4.Group4 setting  ---  HistoryScan layout 
		gridLayoutScanHistroty->addWidget(tableViewCatalog, 0, 0, 1, 1);
		gridLayoutScanHistroty->addWidget(tableViewDetails, 0, 1, 1, 1); 
		{
			tableViewCatalog->setSelectionBehavior(QAbstractItemView::SelectRows);
			tableViewDetails->setSelectionBehavior(QAbstractItemView::SelectRows);
			tableViewCatalog->setModel(tableModelCatalog);
			tableViewDetails->setModel(queryModelDetails);
			connect(tableViewCatalog, &QTableView::clicked, this, &DemoCQScan::tableViewCatalog_clicked);
			connect(tableViewDetails, &QTableView::clicked, this, &DemoCQScan::tableViewDetails_clicked);
			connect(tableViewDetails, &QTableView::doubleClicked, this, &DemoCQScan::tableViewDetails_doubleClicked);

			tableModelCatalog->setTable("tb_scan_catalog");
			tableModelCatalog->sort(0, Qt::DescendingOrder);
			tableModelCatalog->setEditStrategy(QSqlTableModel::OnManualSubmit);
			if (!tableModelCatalog->select()) { QMessageBox::information(this, "", tableModelCatalog->lastError().text()); return; }

			tableModelCatalog->setHeaderData(0, Qt::Horizontal, "扫描主键");
			tableModelCatalog->setHeaderData(1, Qt::Horizontal, "扫描方向");
			tableModelCatalog->setHeaderData(2, Qt::Horizontal, "扫描速率");
			tableModelCatalog->setHeaderData(3, Qt::Horizontal, "扫描长度");
			tableModelCatalog->setHeaderData(4, Qt::Horizontal, "扫描步长");

			queryModelDetails->sort(0, Qt::DescendingOrder);
		}
		gridLayoutScanHistroty->setColumnStretch(0, 3);
		gridLayoutScanHistroty->setColumnStretch(1, 2);

		//5.Group5 setting  ---  PortManagement layout
		gridLayoutPortSetting->addWidget(new QLabel("数据串口", groupBoxPortSetting), 0, 0, 1, 1);
		gridLayoutPortSetting->addWidget(comboBoxDataPort, 0, 1, 1, 1);
		gridLayoutPortSetting->addWidget(pushButtonOpenDataPort, 0, 2, 1, 1);
		gridLayoutPortSetting->addWidget(new QLabel("控制串口", groupBoxPortSetting), 1, 0, 1, 1);
		gridLayoutPortSetting->addWidget(comboBoxCtrlPort, 1, 1, 1, 1);
		gridLayoutPortSetting->addWidget(pushButtonOpenCtrlPort, 1, 2, 1, 1);
		gridLayoutPortSetting->addWidget(pushButtonOpenEthernet, 2, 0, 1, 3);
		{
			QList<QSerialPortInfo> listSerialPortInfo = QSerialPortInfo::availablePorts();
			for (int i = 0; i < listSerialPortInfo.size(); ++i) comboBoxDataPort->addItem(listSerialPortInfo[i].portName());
			for (int i = 0; i < listSerialPortInfo.size(); ++i) comboBoxCtrlPort->addItem(listSerialPortInfo[i].portName());
			connect(pushButtonOpenDataPort, &QPushButton::clicked, this, &DemoCQScan::pushButtonOpenDataPort_clickded);
			connect(pushButtonOpenCtrlPort, &QPushButton::clicked, this, &DemoCQScan::pushButtonOpenCtrlPort_clickded);
			connect(pushButtonOpenEthernet, &QPushButton::clicked, this, &DemoCQScan::pushButtonOpenEthernet_clickded);
		}

		//6.Group6 setting  ---  RealtimePose layout
		gridLayoutCurrentPose->addWidget(new QLabel("X 坐标", groupBoxCurrentPose), 0, 0, 1, 1);
		gridLayoutCurrentPose->addWidget(lineEditXAxis, 0, 1, 1, 1);
		gridLayoutCurrentPose->addWidget(new QLabel("Y 坐标", groupBoxCurrentPose), 1, 0, 1, 1);
		gridLayoutCurrentPose->addWidget(lineEditYAxis, 1, 1, 1, 1);
		gridLayoutCurrentPose->addWidget(new QLabel("Z 坐标", groupBoxCurrentPose), 2, 0, 1, 1);
		gridLayoutCurrentPose->addWidget(lineEditZAxis, 2, 1, 1, 1);
		gridLayoutCurrentPose->addWidget(new QLabel("旋转角", groupBoxCurrentPose), 3, 0, 1, 1);
		gridLayoutCurrentPose->addWidget(lineEditRotate, 3, 1, 1, 1);

		//7.Group7 setting  ---  PoseSetting layout
		gridLayoutPoseSetting->addWidget(pushButtonMoveRight, 0, 0, 1, 1);
		gridLayoutPoseSetting->addWidget(pushButtonMoveLeft, 0, 1, 1, 1);
		gridLayoutPoseSetting->addWidget(pushButtonMoveForward, 1, 0, 1, 1);
		gridLayoutPoseSetting->addWidget(pushButtonMoveBackward, 1, 1, 1, 1);
		gridLayoutPoseSetting->addWidget(pushButtonMoveUp, 2, 0, 1, 1);
		gridLayoutPoseSetting->addWidget(pushButtonMoveDown, 2, 1, 1, 1);
		gridLayoutPoseSetting->addWidget(pushButtonRotateClockwise, 3, 0, 1, 1);
		gridLayoutPoseSetting->addWidget(pushButtonRotateAntiClockwise, 3, 1, 1, 1);
		{
			connect(pushButtonMoveRight, &QPushButton::pressed, [this]()->void {motionCtrl_event(CMD_SJ_GODIST, 0x0, 200, 5); });
			connect(pushButtonMoveLeft, &QPushButton::pressed, [this]()->void {motionCtrl_event(CMD_SJ_GODIST, 0x0, -200, 5); });
			connect(pushButtonMoveForward, &QPushButton::pressed, [this]()->void {motionCtrl_event(CMD_SJ_GODIST, 0x1, 200, 5); });
			connect(pushButtonMoveBackward, &QPushButton::pressed, [this]()->void {motionCtrl_event(CMD_SJ_GODIST, 0x1, -200, 5); });
			connect(pushButtonMoveUp, &QPushButton::pressed, [this]()->void {motionCtrl_event(CMD_SJ_GODIST, 0x2, 200, 5); });
			connect(pushButtonMoveDown, &QPushButton::pressed, [this]()->void {motionCtrl_event(CMD_SJ_GODIST, 0x2, -200, 5); });
			connect(pushButtonRotateClockwise, &QPushButton::pressed, [this]()->void {motionCtrl_event(CMD_SJ_GODIST, 0x3, -2, 0.06); });
			connect(pushButtonRotateAntiClockwise, &QPushButton::pressed, [this]()->void {motionCtrl_event(CMD_SJ_GODIST, 0x3, 2, 0.06); });

			connect(pushButtonMoveRight, &QPushButton::released, [this]()->void {motionCtrl_event(CMD_SJ_GO_STOP, 0x0, 0xFF, 0xFF); });
			connect(pushButtonMoveLeft, &QPushButton::released, [this]()->void {motionCtrl_event(CMD_SJ_GO_STOP, 0x0, 0xFF, 0xFF); });
			connect(pushButtonMoveForward, &QPushButton::released, [this]()->void {motionCtrl_event(CMD_SJ_GO_STOP, 0x1, 0xFF, 0xFF); });
			connect(pushButtonMoveBackward, &QPushButton::released, [this]()->void {motionCtrl_event(CMD_SJ_GO_STOP, 0x1, 0xFF, 0xFF); });
			connect(pushButtonMoveUp, &QPushButton::released, [this]()->void {motionCtrl_event(CMD_SJ_GO_STOP, 0x2, 0xFF, 0xFF); });
			connect(pushButtonMoveDown, &QPushButton::released, [this]()->void {motionCtrl_event(CMD_SJ_GO_STOP, 0x2, 0xFF, 0xFF); });
			connect(pushButtonRotateClockwise, &QPushButton::released, [this]()->void {motionCtrl_event(CMD_SJ_GO_STOP, 0x3, 0xFF, 0xFF); });
			connect(pushButtonRotateAntiClockwise, &QPushButton::released, [this]()->void {motionCtrl_event(CMD_SJ_GO_STOP, 0x3, 0xFF, 0xFF); });
		}

		//8.Group8 setting  ---  SampleReset layout
		gridLayoutGroupSampleScan->addWidget(pushButtonSample, 0, 0, 1, 1);
		gridLayoutGroupSampleScan->addWidget(pushButtonReset, 1, 0, 1, 1);
		{
			pushButtonReset->setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred));
			pushButtonSample->setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred));
			connect(pushButtonSample, &QPushButton::clicked, this, &DemoCQScan::pushButtonSample_clicked);
			connect(pushButtonReset, &QPushButton::clicked, [this]()->void { motionCtrl_event(CMD_DEVRST, 0xFF, 0xFF, 0xFF); });
		}
	}

public://UI members
	QGridLayout *gridLayoutWidgetMain = new QGridLayout(this);
	QWidget *widget3D;
	QTabWidget *tabWidget = new QTabWidget(this);
	QWidget *widgetScanInitial = new QWidget(tabWidget);
	QWidget *widgetScanAutomatic = new QWidget(tabWidget);
	QWidget *widgetScanHistroty = new QWidget(tabWidget);

	QGridLayout *gridLayoutScanInitial = new QGridLayout(widgetScanInitial);
	QGroupBox *groupBoxPortSetting = new QGroupBox("通信设置", widgetScanInitial);
	QGroupBox *groupBoxCurrentPose = new QGroupBox("当前姿态", widgetScanInitial);
	QGroupBox *groupBoxPoseSetting = new QGroupBox("姿态设置", widgetScanInitial);
	QGroupBox *groupBoxSampleScan = new QGroupBox("采样重置", widgetScanInitial);

	QGridLayout *gridLayoutScanAutomatic = new QGridLayout(widgetScanAutomatic);
	QComboBox *comboBoxScanAxis = new QComboBox(widgetScanAutomatic);
	QDoubleSpinBox *doubleSpinBoxScanSpeed = new QDoubleSpinBox(widgetScanAutomatic);
	QSpinBox *spinBoxScanDistance = new QSpinBox(widgetScanAutomatic);
	QComboBox *comboBoxScanStep = new QComboBox(widgetScanAutomatic);
	QPushButton *pushButtonAutomatic = new QPushButton("一键扫描", widgetScanAutomatic);

	QGridLayout *gridLayoutScanHistroty = new QGridLayout(widgetScanHistroty);
	QTableView *tableViewCatalog = new QTableView(widgetScanHistroty);
	QTableView *tableViewDetails = new QTableView(widgetScanHistroty);

	QGridLayout *gridLayoutPortSetting = new QGridLayout(groupBoxPortSetting);
	QComboBox *comboBoxDataPort = new QComboBox(groupBoxPortSetting);
	QPushButton *pushButtonOpenDataPort = new QPushButton("打开", groupBoxPortSetting);
	QComboBox *comboBoxCtrlPort = new QComboBox(groupBoxPortSetting);
	QPushButton *pushButtonOpenCtrlPort = new QPushButton("打开", groupBoxPortSetting);
	QPushButton *pushButtonOpenEthernet = new QPushButton("打开激光设备", groupBoxPortSetting);
	
	QGridLayout *gridLayoutCurrentPose = new QGridLayout(groupBoxCurrentPose);
	QLineEdit *lineEditXAxis = new QLineEdit("-1.000", groupBoxCurrentPose);
	QLineEdit *lineEditYAxis = new QLineEdit("-1.000", groupBoxCurrentPose);
	QLineEdit *lineEditZAxis = new QLineEdit("-1.000", groupBoxCurrentPose);
	QLineEdit *lineEditRotate = new QLineEdit("-1.000", groupBoxCurrentPose);

	QGridLayout *gridLayoutPoseSetting = new QGridLayout(groupBoxPoseSetting);
	QPushButton *pushButtonMoveRight = new QPushButton("向右移", groupBoxPoseSetting);
	QPushButton *pushButtonMoveLeft = new QPushButton("向左移", groupBoxPoseSetting);
	QPushButton *pushButtonMoveForward = new QPushButton("向前移", groupBoxPoseSetting);
	QPushButton *pushButtonMoveBackward = new QPushButton("向后移", groupBoxPoseSetting);
	QPushButton *pushButtonMoveUp = new QPushButton("向上移", groupBoxPoseSetting);
	QPushButton *pushButtonMoveDown = new QPushButton("向下移", groupBoxPoseSetting);
	QPushButton *pushButtonRotateClockwise = new QPushButton("顺旋转", groupBoxPoseSetting);
	QPushButton *pushButtonRotateAntiClockwise = new QPushButton("逆旋转", groupBoxPoseSetting);

	QGridLayout *gridLayoutGroupSampleScan = new QGridLayout(groupBoxSampleScan);
	QPushButton *pushButtonSample = new QPushButton("采样", groupBoxSampleScan);
	QPushButton *pushButtonReset = new QPushButton("重置", groupBoxSampleScan);

public://Data members
	QSerialPort portXYZR;//Used by: pushButtonOpenDataPort_clickded, portXYZR_readyRead
	QSerialPort portCtrl;//Used by: pushButtonOpenCtrlPort_clickded, portXYZR_readyRead, motionCtrl_event
	float realtimeX, lasttimeX, initialX;//Used by: portXYZR_readyRead, pushButtonAutomatic_clicked, timerContinousScan_timeout
	float realtimeY, lasttimeY, initialY;//Used by: portXYZR_readyRead, pushButtonAutomatic_clicked, timerContinousScan_timeout
	float realtimeZ, lasttimeZ, initialZ;//Used by: portXYZR_readyRead, pushButtonAutomatic_clicked, timerContinousScan_timeout
	float realtimeR, lasttimeR, initialR;//Used by: portXYZR_readyRead, pushButtonAutomatic_clicked, timerContinousScan_timeout
	long timeid;//Used by: pushButtonSample_clicked, pushButtonAutomatic_clicked, timerContinousScan_timeout
	long scanid;//Used by: pushButtonAutomatic_clicked, timerContinousScan_timeout

	CInterfaceLLT *m_pLLT = NULL;//After being initialized by initScanDevice, used by timerContinousScan_timeout
	TScannerType m_tscanCONTROLType;//After being initialized by initScanDevice, used by timerContinousScan_timeout
	uint m_uiResolution;//After being initialized by initScanDevice, used by timerContinousScan_timeout
	QTimer *timerContinousScan = new QTimer(this);
	int scanMode;//Used by: pushButtonOpenEthernet_clickded, tabWidget_tabBarClicked, pushButtonSample_clicked, pushButtonAutomatic_clicked, timerContinousScan_timeout

	Q3DScatter *scatter3D = new Q3DScatter();

	QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "connect_name_of_sqlite");
	QSqlTableModel *tableModelCatalog = new QSqlTableModel(this, db);
	QSqlQueryModel *queryModelDetails = new QSqlQueryModel(this);	
};

#endif