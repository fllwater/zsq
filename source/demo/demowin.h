#include "window/windowaaa.h"

#ifndef __demowin_h__
#define __demowin_h__

namespace demowin
{//所有图形界面Demo

	class DialogOpenCamera : public QDialog
	{
	public:
		typedef struct OpenCameraParams
		{
			int cameraId;
			int frameCols;
			int frameRows;
			int frameRate;
			int pixelFormat;
			double pixelAspectRatio;
			bool useOpencv;
			QString saveDir;
		} OpenCameraParams;
	signals: void sendData(OpenCameraParams params);

	public://2.功能实现
		void comboBoxCamera_currentIndexChanged(int ind)
		{
			QCamera camera(listCameraInfo[ind]); camera.load();
			QList<QCameraViewfinderSettings> listCameraViewfinderSettings = camera.supportedViewfinderSettings(); camera.unload();
			tableWidgetSupportedSetting->setRowCount(listCameraViewfinderSettings.size() + 1);
			for (int i = 0; i < listCameraViewfinderSettings.size(); ++i)
			{
				listCameraViewfinderSettings[i].resolution().width();
				tableWidgetSupportedSetting->setItem(i, 0, new QTableWidgetItem(QString::number(listCameraViewfinderSettings[i].resolution().width())));
				tableWidgetSupportedSetting->setItem(i, 1, new QTableWidgetItem(QString::number(listCameraViewfinderSettings[i].resolution().height())));
				tableWidgetSupportedSetting->setItem(i, 2, new QTableWidgetItem(QString::number((int)listCameraViewfinderSettings[i].maximumFrameRate())));
				tableWidgetSupportedSetting->setItem(i, 3, new QTableWidgetItem(QString::number(listCameraViewfinderSettings[i].pixelFormat())));
				tableWidgetSupportedSetting->setItem(i, 4, new QTableWidgetItem(QString::number(listCameraViewfinderSettings[i].pixelAspectRatio().width() / (float)listCameraViewfinderSettings[i].pixelAspectRatio().height())));
				tableWidgetSupportedSetting->item(i, 0)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
				tableWidgetSupportedSetting->item(i, 1)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
				tableWidgetSupportedSetting->item(i, 2)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
				tableWidgetSupportedSetting->item(i, 3)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
				tableWidgetSupportedSetting->item(i, 4)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
			}
			tableWidgetSupportedSetting->resizeRowsToContents();
			tableWidgetSupportedSetting->resizeColumnsToContents();
		}
		void comboBoxSaveDir_activated() { comboBoxSaveDir->setEditText(QFileDialog::getExistingDirectory(this, "Save Directory", "./../data/tmp")); }
		void tableWidgetSupportedSetting_cellClicked(int i, int j)
		{
			spinBoxFrameCols->setValue(tableWidgetSupportedSetting->item(i, 0)->text().toInt());
			spinBoxFrameRows->setValue(tableWidgetSupportedSetting->item(i, 1)->text().toInt());
			spinBoxFrameRate->setValue(tableWidgetSupportedSetting->item(i, 2)->text().toInt());
			spinBoxPixelFormat->setValue(tableWidgetSupportedSetting->item(i, 3)->text().toInt());
			doubleSpinBoxPixelAspectRatio->setValue(tableWidgetSupportedSetting->item(i, 4)->text().toDouble());
		}
		void dialogButtonBox_accepted()
		{
			OpenCameraParams params;
			params.cameraId = comboBoxCamera->currentIndex();
			params.frameCols = spinBoxFrameCols->value();
			params.frameRows = spinBoxFrameRows->value();
			params.frameRate = spinBoxFrameRate->value();
			params.pixelFormat = spinBoxPixelFormat->value();
			params.pixelAspectRatio = doubleSpinBoxPixelAspectRatio->value();
			params.useOpencv = checkBoxUseOpencv->isChecked();
			params.saveDir = comboBoxSaveDir->currentText();
			accept(); emit sendData(params);
		}


	public://1.界面设计
		QFormLayout *formLayoutMain = new QFormLayout(this);
		QComboBox *comboBoxCamera = new QComboBox(this);
		QComboBox *comboBoxSaveDir = new QComboBox(this);
		QSpinBox *spinBoxFrameRows = new QSpinBox(this);
		QSpinBox *spinBoxFrameCols = new QSpinBox(this);
		QSpinBox *spinBoxFrameRate = new QSpinBox(this);
		QSpinBox *spinBoxPixelFormat = new QSpinBox(this);
		QDoubleSpinBox *doubleSpinBoxPixelAspectRatio = new QDoubleSpinBox(this);
		QTableWidget *tableWidgetSupportedSetting = new QTableWidget(1, 5, this);
		QCheckBox *checkBoxUseOpencv = new QCheckBox("Use opencv", this);
		QDialogButtonBox *dialogButtonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
		QList<QCameraInfo> listCameraInfo = QCameraInfo::availableCameras();
		DialogOpenCamera(QWidget *parent) : QDialog(parent) { createMainUI(); }
		void createMainUI()
		{
			//0.基本设置
			setWindowTitle(typeid(this).name());

			//1.顶级布局
			formLayoutMain->addRow("Camera: ", comboBoxCamera);
			formLayoutMain->addRow("SaveTo: ", comboBoxSaveDir);
			formLayoutMain->addRow("FrameRows: ", spinBoxFrameRows);
			formLayoutMain->addRow("FrameCols: ", spinBoxFrameCols);
			formLayoutMain->addRow("FrameRate: ", spinBoxFrameRate);
			formLayoutMain->addRow("PixelFormat: ", spinBoxPixelFormat);
			formLayoutMain->addRow("PixelAspectRatio: ", doubleSpinBoxPixelAspectRatio);
			formLayoutMain->addRow("SupportedSetting: ", tableWidgetSupportedSetting);
			formLayoutMain->addRow(checkBoxUseOpencv, dialogButtonBox);

			//8.部件初始化
			for (int i = 0; i < listCameraInfo.size(); ++i) comboBoxCamera->addItem(listCameraInfo[i].description());
			comboBoxSaveDir->setEditable(true);
			comboBoxSaveDir->addItem("Choose Save Dir");
			comboBoxSaveDir->setCurrentText(QString("./../data/") + aaa::num2string(aaa_ns).c_str());//若不存在槽函数将创建
			spinBoxFrameRows->setRange(1, 10000);
			spinBoxFrameRows->setValue(480);
			spinBoxFrameCols->setRange(1, 10000);
			spinBoxFrameCols->setValue(1280);
			spinBoxFrameRate->setRange(1, 10000);
			spinBoxFrameRate->setValue(1000);
			spinBoxPixelFormat->setRange(1, 10000);
			spinBoxPixelFormat->setValue(QVideoFrame::Format_YUYV);
			doubleSpinBoxPixelAspectRatio->setValue(1.0);
			checkBoxUseOpencv->setChecked(true);

			tableWidgetSupportedSetting->setHorizontalHeaderLabels(QStringList() << tr("FrameCols") << tr("FrameRows") << tr("FrameRate") << tr("PixFormat") << tr("PixelAspectRatio"));
			tableWidgetSupportedSetting->setEditTriggers(QAbstractItemView::NoEditTriggers);
			tableWidgetSupportedSetting->setSelectionBehavior(QAbstractItemView::SelectRows);
			tableWidgetSupportedSetting->setSelectionMode(QAbstractItemView::SingleSelection);
			tableWidgetSupportedSetting->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
			tableWidgetSupportedSetting->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
			comboBoxCamera_currentIndexChanged(0);

			//9.动作关联
			connect(comboBoxCamera, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &DialogOpenCamera::comboBoxCamera_currentIndexChanged);
			connect(comboBoxSaveDir, static_cast<void (QComboBox::*)(int)>(&QComboBox::activated), this, &DialogOpenCamera::comboBoxSaveDir_activated);
			connect(tableWidgetSupportedSetting, &QTableWidget::cellClicked, this, &DialogOpenCamera::tableWidgetSupportedSetting_cellClicked);
			connect(dialogButtonBox, &QDialogButtonBox::accepted, this, &DialogOpenCamera::dialogButtonBox_accepted);
			connect(dialogButtonBox, &QDialogButtonBox::rejected, this, &DialogOpenCamera::reject);
		}
		void resizeEvent(QResizeEvent *event) { this->setFixedSize(this->width(), this->height()); }

	public://0.无独立成员函数: (1)
	public:Q_OBJECT
	};

	class DialogRWSerialPort : public QDialog
	{
	public://3.功能实现	
		void pushButtonPortOpen_clicked()
		{
			if (serialPort.portName() == comboBoxPortName->currentText())
			{
				if (serialPort.isOpen()) QMessageBox::information(this, "Infomation", serialPort.portName() + " was opened ago");
				else if (serialPort.open(QIODevice::ReadWrite)) QMessageBox::information(this, "Infomation", serialPort.portName() + " was opened successfully");
				else QMessageBox::information(this, "Infomation", serialPort.portName() + " failed to be opened");
			}
			else
			{
				if(serialPort.isOpen()) QMessageBox::information(this, "Infomation", "Please close opened serial port: " + serialPort.portName());
				else
				{
					//0.port
					serialPort.setPort(listSerialPortInfo[comboBoxPortName->currentIndex()]);
					//1.baudrate
					if (serialPort.setBaudRate(comboBoxPortBaudRate->currentText().toInt()) == false)
					{
						QMessageBox::information(this, "Infomation", serialPort.portName() + " failed to set baudrate");
						return;
					}
					//2.databits
					if (serialPort.setDataBits((QSerialPort::DataBits)comboBoxPortDataBits->currentText().toInt()) == false)
					{
						QMessageBox::information(this, "Infomation", serialPort.portName() + " failed to set databits");
						return;
					}
					//3.parity
					int parity = comboBoxPortParity->currentIndex(); if (parity > 0) parity += 1;
					if (serialPort.setParity((QSerialPort::Parity)parity) == false)
					{
						QMessageBox::information(this, "Infomation", serialPort.portName() + " failed to set parity");
						return;
					}
					//4.stopbits
					int stopbits = comboBoxPortStopBits->currentIndex();
					if (stopbits == 0) serialPort.setStopBits(QSerialPort::OneStop);
					else if (stopbits == 1) serialPort.setStopBits(QSerialPort::OneAndHalfStop);
					else serialPort.setStopBits(QSerialPort::TwoStop);
					//5.flowcontrol
					if (serialPort.setFlowControl((QSerialPort::FlowControl)comboBoxPortFlowControl->currentIndex()) == false)
					{
						QMessageBox::information(this, "Infomation", serialPort.portName() + " failed to set databits");
						return;
					}
					//6.open it
					if (serialPort.open(QIODevice::ReadWrite)) QMessageBox::information(this, "Infomation", serialPort.portName() + " was opened successfully");
					else QMessageBox::information(this, "Infomation", serialPort.portName() + " failed to be opened");
				}
			}
		}

		void pushButtonPortClose_clicked()
		{
			if (serialPort.isOpen()) { serialPort.close(); QMessageBox::information(this, "Infomation", serialPort.portName() + " was close successfully"); }
			else QMessageBox::information(this, "Infomation", "No serialport was opened ago");
		}

		void pushButtonPortSend_clicked() { serialPort.write(textEditPortSendData->toPlainText().toUtf8());	}

		void serialPort_readyRead() { textEditPortReceiveData->setPlainText(textEditPortReceiveData->toPlainText() + QString::fromLocal8Bit(serialPort.readAll())); }

		void serialPort_bytesWritten(long bytes) { QMessageBox::information(this, "Information", QString("Some data was sended out and the size: ") + aaa::num2string(bytes).c_str()); }

		void serialPort_error(QSerialPort::SerialPortError error) { if (error > 0) QMessageBox::warning(this, "Warning", QString("Error occured and the error code: ") + aaa::num2string(error).c_str()); }

	public://2.功能实现
		void comboBoxPortName_currentIndexChanged(int ind)
		{
			textEditPortCurSetting->setText(
				"Port Name: " + listSerialPortInfo[ind].portName() +
				"\nSystem Location: " + listSerialPortInfo[ind].systemLocation() +
				"\nIs Busy: " + (listSerialPortInfo[ind].isBusy() ? "true" : "false") +
				"\nIs Null: " + (listSerialPortInfo[ind].isNull() ? "true" : "false") +
				"\nhas ProductIdentifier: " + (listSerialPortInfo[ind].hasProductIdentifier() ? "true" : "false") +
				"\nhas VendorIdentifier: " + (listSerialPortInfo[ind].hasVendorIdentifier() ? "true" : "false") +
				"\nDescription: " + listSerialPortInfo[ind].description() +
				"\nSerial Number: " + listSerialPortInfo[ind].serialNumber() +
				"\nProduct Identifier: " + listSerialPortInfo[ind].productIdentifier() +
				"\nVendor Identifier: " + listSerialPortInfo[ind].vendorIdentifier() +
				"\nManufacturer: " + listSerialPortInfo[ind].manufacturer());
		}

	public://1.界面设计
		QGridLayout *gridLayoutMain = new QGridLayout(this);
		QLabel *labelPortSetting = new QLabel("SerialPort Setting", this);//0

		QLabel *labelPortName = new QLabel("Name", this);						QComboBox *comboBoxPortName = new QComboBox(this);//1
		QLabel *labelPortBaudRate = new QLabel("BaudRate", this);				QComboBox *comboBoxPortBaudRate = new QComboBox(this);//2
		QLabel *labelPortDataBits = new QLabel("DataBits", this);				QComboBox *comboBoxPortDataBits = new QComboBox(this);//3
		QLabel *labelPortParity = new QLabel("Parity", this);					QComboBox *comboBoxPortParity = new QComboBox(this);//4
		QLabel *labelPortStopBits = new QLabel("StopBits", this);				QComboBox *comboBoxPortStopBits = new QComboBox(this);//5
		QLabel *labelPortFlowControl = new QLabel("FlowControl", this);			QComboBox *comboBoxPortFlowControl = new QComboBox(this);//6
		QLabel *labelPortReceiveAs = new QLabel("ReceiveAs", this);				QComboBox *comboBoxPortReceiveAs = new QComboBox(this);//7
		QLabel *labelPortSendAs = new QLabel("SendAs", this);					QComboBox *comboBoxPortSendAs = new QComboBox(this);//8
		QLabel *labelPortCurSetting = new QLabel("\nSerialPort Information", this);//9
		QTextEdit *textEditPortCurSetting = new QTextEdit("null", this);//10

		QLabel *labelSeperator = new QLabel("<--->", this);

		QLabel *labelPortReceiveData = new QLabel("Receiving Area", this);
		QTextEdit *textEditPortReceiveData = new QTextEdit("null", this);
		QLabel *labelPortSendData = new QLabel("\nSending Area", this);
		QTextEdit *textEditPortSendData = new QTextEdit("null", this);
		QPushButton *pushButtonPortOpen = new QPushButton("Open", this);
		QPushButton *pushButtonPortClose = new QPushButton("Close", this);
		QPushButton *pushButtonPortSend = new QPushButton("Send", this);

		QList<QSerialPortInfo> listSerialPortInfo = QSerialPortInfo::availablePorts();
		QList<int> listBaudRate = QSerialPortInfo::standardBaudRates();
		QSerialPort serialPort;

		DialogRWSerialPort(QWidget *parent) : QDialog(parent) { createMainUI(); }
		void createMainUI()
		{
			//0.基本设置
			setWindowTitle(typeid(this).name()); 

			//1.顶级布局
			gridLayoutMain->addWidget(labelPortSetting, 0, 0, 1, 2);
			gridLayoutMain->addWidget(labelPortName, 1, 0, Qt::AlignLeft);					gridLayoutMain->addWidget(comboBoxPortName, 1, 1, Qt::AlignRight);			comboBoxPortName->setFixedWidth(labelPortSetting->width());
			gridLayoutMain->addWidget(labelPortBaudRate, 2, 0, Qt::AlignLeft);				gridLayoutMain->addWidget(comboBoxPortBaudRate, 2, 1, Qt::AlignRight);		comboBoxPortBaudRate->setFixedWidth(labelPortSetting->width());
			gridLayoutMain->addWidget(labelPortDataBits, 3, 0, Qt::AlignLeft);				gridLayoutMain->addWidget(comboBoxPortDataBits, 3, 1, Qt::AlignRight);		comboBoxPortDataBits->setFixedWidth(labelPortSetting->width());
			gridLayoutMain->addWidget(labelPortParity, 4, 0, Qt::AlignLeft);				gridLayoutMain->addWidget(comboBoxPortParity, 4, 1, Qt::AlignRight);		comboBoxPortParity->setFixedWidth(labelPortSetting->width());
			gridLayoutMain->addWidget(labelPortStopBits, 5, 0, Qt::AlignLeft);				gridLayoutMain->addWidget(comboBoxPortStopBits, 5, 1, Qt::AlignRight);		comboBoxPortStopBits->setFixedWidth(labelPortSetting->width());
			gridLayoutMain->addWidget(labelPortFlowControl, 6, 0, Qt::AlignLeft);			gridLayoutMain->addWidget(comboBoxPortFlowControl, 6, 1, Qt::AlignRight);	comboBoxPortFlowControl->setFixedWidth(labelPortSetting->width());
			gridLayoutMain->addWidget(labelPortReceiveAs, 7, 0, Qt::AlignLeft);				gridLayoutMain->addWidget(comboBoxPortReceiveAs, 7, 1, Qt::AlignRight);	comboBoxPortReceiveAs->setFixedWidth(labelPortSetting->width());
			gridLayoutMain->addWidget(labelPortSendAs, 8, 0, Qt::AlignLeft);				gridLayoutMain->addWidget(comboBoxPortSendAs, 8, 1, Qt::AlignRight);		comboBoxPortSendAs->setFixedWidth(labelPortSetting->width());
			gridLayoutMain->addWidget(labelPortCurSetting, 9, 0, 1, 2);
			gridLayoutMain->addWidget(textEditPortCurSetting, 10, 0, 8, 2);

			gridLayoutMain->addWidget(labelSeperator, 0, 2, 19, 1);

			gridLayoutMain->addWidget(labelPortReceiveData, 0, 3, 1, 3);
			gridLayoutMain->addWidget(textEditPortReceiveData, 1, 3, 8, 3);
			gridLayoutMain->addWidget(labelPortSendData, 9, 3, 1, 3);
			gridLayoutMain->addWidget(textEditPortSendData, 10, 3, 8, 3);

			gridLayoutMain->addWidget(pushButtonPortOpen, 18, 3);
			gridLayoutMain->addWidget(pushButtonPortClose, 18, 4);
			gridLayoutMain->addWidget(pushButtonPortSend, 18, 5);

			//8.部件初始化
			for (int i = 0; i < listSerialPortInfo.size(); ++i) comboBoxPortName->addItem(listSerialPortInfo[i].portName());
			for (int i = 0; i < listBaudRate.size(); ++i) comboBoxPortBaudRate->addItem(aaa::num2string(listBaudRate[i]).c_str());
			for (int i = 8; i > 4; --i) comboBoxPortDataBits->addItem(aaa::num2string(i).c_str());
			comboBoxPortParity->addItem("None"); comboBoxPortParity->addItem("Even"); comboBoxPortParity->addItem("Odd"); comboBoxPortParity->addItem("Space"); comboBoxPortParity->addItem("Mark");
			comboBoxPortStopBits->addItem("1"); comboBoxPortStopBits->addItem("1.5"); comboBoxPortStopBits->addItem("2");
			comboBoxPortFlowControl->addItem("None"); comboBoxPortFlowControl->addItem("RTS/CTS"); comboBoxPortFlowControl->addItem("XON/XOFF");
			comboBoxPortReceiveAs->addItem("Text"); comboBoxPortReceiveAs->addItem("HEX"); comboBoxPortReceiveAs->setEnabled(false);
			comboBoxPortSendAs->addItem("Text"); comboBoxPortSendAs->addItem("HEX"); comboBoxPortSendAs->setEnabled(false);
			textEditPortCurSetting->setEnabled(false);
			textEditPortReceiveData->setEnabled(false);
			comboBoxPortName_currentIndexChanged(0);

			//9.动作关联
			connect(comboBoxPortName, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &DialogRWSerialPort::comboBoxPortName_currentIndexChanged);
			connect(pushButtonPortOpen, &QPushButton::clicked, this, &DialogRWSerialPort::pushButtonPortOpen_clicked);
			connect(pushButtonPortClose, &QPushButton::clicked, this, &DialogRWSerialPort::pushButtonPortClose_clicked);
			connect(pushButtonPortSend, &QPushButton::clicked, this, &DialogRWSerialPort::pushButtonPortSend_clicked);

			connect(&serialPort, &QSerialPort::readyRead, this, &DialogRWSerialPort::serialPort_readyRead);
			connect(&serialPort, &QSerialPort::bytesWritten, this, &DialogRWSerialPort::serialPort_bytesWritten);
			connect(&serialPort, static_cast<void(QSerialPort::*)(QSerialPort::SerialPortError)>(&QSerialPort::error), this, &DialogRWSerialPort::serialPort_error);
		}
		void resizeEvent(QResizeEvent *event) { this->setFixedSize(this->width(), this->height()); }

	public://0.无独立成员函数: (1)
	//public:Q_OBJECT
	};

	class DialogUseSqlite : public QDialog
	{
	public://3.功能实现

		class DelegateUseSqlite : public QSqlRelationalDelegate
		{
		public:
			DelegateUseSqlite(QObject *parent) : QSqlRelationalDelegate(parent) {}
			void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
			{
				if (index.column() != 6) QSqlRelationalDelegate::paint(painter, option, index);
				else
				{
					QPixmap pixmap(index.data().toString());

					QPalette::ColorGroup cg = (option.state & QStyle::State_Enabled) ? (option.state & QStyle::State_Active) ? QPalette::Normal : QPalette::Inactive : QPalette::Disabled;
					if (option.state & QStyle::State_Selected) painter->fillRect(option.rect, option.palette.color(cg, QPalette::Highlight));

					int x = option.rect.x();
					int y = option.rect.y() + (option.rect.height() / 2) - (pixmap.height() / 2);
					painter->drawPixmap(x, y, pixmap);
					drawFocus(painter, option, option.rect.adjusted(0, 0, -1, -1)); // since we draw the grid ourselves
				}
			}
		};

	public://2.功能实现
		void tableViewBooks_clicked(QModelIndex modelIndex)
		{
			QSqlRecord record = rtableModel->record(modelIndex.row());
			lineEditIdentifier->setText(record.value(1).toString());
			lineEditFullname->setText(record.value(2).toString());
			lineEditBirthdate->setText(record.value(3).toString());
			lineEditEducation->setText(record.value(4).toString());
			lineEditPosition->setText(record.value(5).toString());
			lineEditHead->setText(record.value(6).toString());
			lableStatusTips->setText("Current employee's id: " + record.value(0).toString());
		}

		void pushButtonSearch_clicked()
		{
			QString filter;
			if (!lineEditIdentifier->text().isEmpty()) filter += "identifier='" + lineEditIdentifier->text() + "' and ";
			if (!lineEditFullname->text().isEmpty()) filter += "fullname='" + lineEditFullname->text() + "' and ";
			if (!lineEditBirthdate->text().isEmpty()) filter += "birthdate='" + lineEditBirthdate->text() + "' and ";
			if (!lineEditEducation->text().isEmpty())
			{
				QSqlQuery query("select * from tb_education where name='" + lineEditEducation->text() + "'", db);
				query.exec();
				query.first();
				if (query.isValid()) filter += "education='" + query.value(0).toString() + "' and ";
				else filter += "education='' and ";

			}
			if (!lineEditPosition->text().isEmpty())
			{
				QSqlQuery query("select * from tb_position where name='" + lineEditPosition->text() + "'", db);
				query.exec();
				query.first();
				if (query.isValid())  filter += "position='" + query.value(0).toString() + "' and ";
				else filter += "position='' and ";
			}
			if (!lineEditHead->text().isEmpty()) filter += "head='" + lineEditHead->text() + "'";
			if (filter.endsWith(" and ")) filter = filter.left(filter.length() - 5);

			rtableModel->setFilter(filter);
			if (!rtableModel->select()) { QMessageBox::information(this, "", rtableModel->lastError().text()); return; }
			lableStatusTips->setText("Current select's filter: " + filter);
		}
		void pushButtonModify_clicked()
		{
			if (!tableViewBooks->currentIndex().isValid()) { QMessageBox::information(this, "", "Choose the row to be modified"); return; }
			if (rtableModel->record(tableViewBooks->currentIndex().row()).value(0) == 1) { QMessageBox::information(this, "", "The first row is reserved and cannot be modified"); return; }
			QSqlRecord record = rtableModel->record(tableViewBooks->currentIndex().row());
			record.setValue(1, lineEditIdentifier->text());
			record.setValue(2, lineEditFullname->text());
			record.setValue(3, lineEditBirthdate->text());
			{
				QSqlQuery query("select * from tb_education where name='" + lineEditEducation->text() + "'", db);
				query.exec();
				query.first();
				if (query.isValid()) record.setValue(4, query.value(0).toInt());
				else { QMessageBox::information(this, "", "No exist the education: " + lineEditEducation->text()); return; }
			}
			{
				QSqlQuery query("select * from tb_position where name='" + lineEditPosition->text() + "'", db);
				query.exec();
				query.first();
				if (query.isValid()) record.setValue(5, query.value(0).toInt());
				else { QMessageBox::information(this, "", "No exist the position: " + lineEditPosition->text()); return; }
			}
			record.setValue(6, lineEditHead->text());

			if (rtableModel->setRecord(tableViewBooks->currentIndex().row(), record)) QMessageBox::information(this, "", "Succeed in modifying");
			else QMessageBox::information(this, "", "Fail to modify");
		}
		void pushButtonInsert_clicked()
		{
			QSqlRecord record = rtableModel->record();
			record.setValue(1, lineEditIdentifier->text());
			record.setValue(2, lineEditFullname->text());
			record.setValue(3, lineEditBirthdate->text());
			{
				QSqlQuery query("select * from tb_education where name='" + lineEditEducation->text() + "'", db);
				query.exec();
				query.first();
				if (query.isValid()) record.setValue(4, query.value(0).toInt());
				else { QMessageBox::information(this, "", "No exist the education: " + lineEditEducation->text()); return; }
			}
			{
				QSqlQuery query("select * from tb_position where name='" + lineEditPosition->text() + "'", db);
				query.exec();
				query.first();
				if (query.isValid()) record.setValue(5, query.value(0).toInt());
				else { QMessageBox::information(this, "", "No exist the position: " + lineEditPosition->text()); return; }
			}
			record.setValue(6, lineEditHead->text());

			if (rtableModel->insertRecord(0, record)) QMessageBox::information(this, "", "Succeed in inserting");
			else QMessageBox::information(this, "", "Fail to insert");
		}
		void pushButtonDelete_clicked()
		{
			if (!tableViewBooks->currentIndex().isValid()) { QMessageBox::information(this, "", "Choose the row  to be deleted"); return; }
			if (rtableModel->record(tableViewBooks->currentIndex().row()).value(0) == 1) { QMessageBox::information(this, "", "The first row is reserved and cannot be deleted"); return; }
			if (rtableModel->removeRow(tableViewBooks->currentIndex().row())) QMessageBox::information(this, "", "Succeed in deleting");
			else QMessageBox::information(this, "", "Fail to delete");
		}
		void pushButtonSubmit_clicked()
		{
			if (rtableModel->submitAll())QMessageBox::information(this, "", "Succeed in sumbmitting");
			else QMessageBox::information(this, "", "Fail to submit");
		}
		void pushButtonReload_clicked()
		{
			rtableModel->setFilter("");
			if (!rtableModel->select()) { QMessageBox::information(this, "", rtableModel->lastError().text()); return; }
		}

	public://1.界面设计
		QGridLayout *gridLayoutMain = new QGridLayout(this);
		QGroupBox *groupBoxBooks = new QGroupBox("Dataset", this);
		QGroupBox *groupBoxManage = new QGroupBox("Manage", this);
		QGroupBox *groupBoxStatus = new QGroupBox("Status", this);

		QGridLayout *gridLayoutBooks = new QGridLayout(groupBoxBooks);
		QTableView *tableViewBooks = new QTableView(groupBoxBooks);

		QGridLayout *gridLayoutManage = new QGridLayout(groupBoxManage);
		QLabel *labelIdentifier = new QLabel("identifier", groupBoxManage);
		QLabel *labelFullname = new QLabel("fullname", groupBoxManage);
		QLabel *labelBirthdate = new QLabel("birthdate", groupBoxManage);
		QLabel *labelEducation = new QLabel("education", groupBoxManage);
		QLabel *labelPosition = new QLabel("position", groupBoxManage);
		QLabel *labelHead = new QLabel("head", groupBoxManage);
		QLineEdit *lineEditIdentifier = new QLineEdit("", groupBoxManage);
		QLineEdit *lineEditFullname = new QLineEdit("", groupBoxManage);
		QLineEdit *lineEditBirthdate = new QLineEdit("", groupBoxManage);
		QLineEdit *lineEditEducation = new QLineEdit("", groupBoxManage);
		QLineEdit *lineEditPosition = new QLineEdit("", groupBoxManage);
		QLineEdit *lineEditHead = new QLineEdit("", groupBoxManage);

		QPushButton *pushButtonSearch = new QPushButton("Search", groupBoxManage);
		QPushButton *pushButtonModify = new QPushButton("Modify", groupBoxManage);
		QPushButton *pushButtonInsert = new QPushButton("Insert", groupBoxManage);
		QPushButton *pushButtonDelete = new QPushButton("Delete", groupBoxManage);
		QPushButton *pushButtonSubmit = new QPushButton("Submit", groupBoxManage);
		QPushButton *pushButtonReload = new QPushButton("Reload", groupBoxManage);

		QGridLayout *gridLayoutStatus = new QGridLayout(groupBoxStatus);
		QLabel *lableStatusTips = new QLabel("Here are some tips", groupBoxStatus);

		QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "connect_name_of_sqlite");
		QSqlQuery *query = new QSqlQuery("", db);
		QSqlRelationalTableModel *rtableModel = new QSqlRelationalTableModel(tableViewBooks, db);
		DelegateUseSqlite *delegateUseSqlite = new DelegateUseSqlite(tableViewBooks);

		DialogUseSqlite(QWidget *parent) : QDialog(parent) { createMainUI(); }
		void createMainUI()
		{
			//0.基本设置
			setWindowTitle(typeid(this).name());

			//1.顶级布局
			gridLayoutMain->addWidget(groupBoxBooks, 0, 0);
			gridLayoutMain->addWidget(groupBoxManage, 0, 1);
			gridLayoutMain->addWidget(groupBoxStatus, 1, 0, 1, 2);

			gridLayoutBooks->addWidget(tableViewBooks, 0, 0);
			tableViewBooks->setSelectionBehavior(QAbstractItemView::SelectRows);
			tableViewBooks->verticalHeader()->setDefaultSectionSize(128);
			tableViewBooks->horizontalHeader()->setDefaultSectionSize(128);
			tableViewBooks->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContentsOnFirstShow);

			gridLayoutManage->addWidget(labelIdentifier, 0, 0); gridLayoutManage->addWidget(lineEditIdentifier, 0, 1);
			gridLayoutManage->addWidget(labelFullname, 1, 0); gridLayoutManage->addWidget(lineEditFullname, 1, 1);
			gridLayoutManage->addWidget(labelBirthdate, 2, 0); gridLayoutManage->addWidget(lineEditBirthdate, 2, 1);
			gridLayoutManage->addWidget(labelEducation, 3, 0); gridLayoutManage->addWidget(lineEditEducation, 3, 1);
			gridLayoutManage->addWidget(labelPosition, 4, 0); gridLayoutManage->addWidget(lineEditPosition, 4, 1);
			gridLayoutManage->addWidget(labelHead, 5, 0); gridLayoutManage->addWidget(lineEditHead, 5, 1);

			gridLayoutManage->addWidget(pushButtonSearch, 6, 0, 1, 2);
			gridLayoutManage->addWidget(pushButtonModify, 7, 0, 1, 2);
			gridLayoutManage->addWidget(pushButtonInsert, 8, 0, 1, 2);
			gridLayoutManage->addWidget(pushButtonDelete, 9, 0, 1, 2);
			gridLayoutManage->addWidget(pushButtonSubmit, 10, 0, 1, 2);
			gridLayoutManage->addWidget(pushButtonReload, 11, 0, 1, 2);

			gridLayoutStatus->addWidget(lableStatusTips, 0, 0);

			//8.部件初始化
			db.setDatabaseName("./../data/mysqlite.db");
			if (!db.open()) { QMessageBox::information(this, "", db.lastError().text()); return; }
			rtableModel->setTable("tb_employee");
			rtableModel->sort(0, Qt::DescendingOrder);
			rtableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
			rtableModel->setRelation(4, QSqlRelation("tb_education", "id", "name")); rtableModel->setHeaderData(4, Qt::Horizontal, "education");
			rtableModel->setRelation(5, QSqlRelation("tb_position", "id", "name")); rtableModel->setHeaderData(5, Qt::Horizontal, "position");
			if (!rtableModel->select()) { QMessageBox::information(this, "", rtableModel->lastError().text()); return; }
			tableViewBooks->setModel(rtableModel);
			tableViewBooks->setItemDelegate(delegateUseSqlite);
			tableViewBooks->hideColumn(0);

			//9.动作关联
			connect(tableViewBooks, &QTableView::clicked, this, &DialogUseSqlite::tableViewBooks_clicked);
			connect(pushButtonSearch, &QPushButton::clicked, this, &DialogUseSqlite::pushButtonSearch_clicked);
			connect(pushButtonModify, &QPushButton::clicked, this, &DialogUseSqlite::pushButtonModify_clicked);
			connect(pushButtonInsert, &QPushButton::clicked, this, &DialogUseSqlite::pushButtonInsert_clicked);
			connect(pushButtonDelete, &QPushButton::clicked, this, &DialogUseSqlite::pushButtonDelete_clicked);
			connect(pushButtonSubmit, &QPushButton::clicked, this, &DialogUseSqlite::pushButtonSubmit_clicked);
			connect(pushButtonReload, &QPushButton::clicked, this, &DialogUseSqlite::pushButtonReload_clicked);
		}
		void resizeEvent(QResizeEvent *event) { this->setFixedSize(this->width(), this->height()); }
	};

	class MyMainWindow : public QMainWindow
	{
	public://7.3 进程通信项目: 使用Sqlite
		void actionUseSqlite_triggered()
		{
			DialogUseSqlite *dialogUseSqlite = new DialogUseSqlite(this);
			dialogUseSqlite->exec();
			delete dialogUseSqlite;
		}
	public://7.2 进程通信项目: 读写串口
		void actionRWSerialPort_triggered()
		{
			DialogRWSerialPort *dialogRWSerialPort = new DialogRWSerialPort(this);
			dialogRWSerialPort->exec();
			delete dialogRWSerialPort;
		}

	public://7.1 进程通信项目: 运行进程
		QProcess processTry;
		QImage *imageTryProcess = 0;
		int yimageTryProcess = -10;
		void actionTryProcess_triggered()
		{
			//0.
			if (imageTryProcess != 0) { QMessageBox::information(this, "Infomation", "Certain process is running"); return; }
			imageTryProcess = new QImage(800, 600, QImage::Format_ARGB32);
			imageTryProcess->fill(QColor(225, 225, 225));
			yimageTryProcess = -10;

			//1.
			QString processPath = QFileDialog::getOpenFileName(this, "Input application path", "./../data/");
			if (processPath == "") return;
			QString processParams = QInputDialog::getMultiLineText(this, "Input application params", "Enter params (One per line): ", "aaa\nbbb\nccc");
			if (processParams == "") return;

			//2.
			string processParams_= processParams.toStdString() + "\n";// in case users don't input the enter key at the end
			size_t indNext = 0;
			QStringList listParam;
			while (1)
			{
				size_t indDst = processParams_.find_first_of('\n', indNext);
				if (indDst == string::npos) break;
				listParam.push_back(processParams_.substr(indNext, indDst - indNext).c_str());
				indNext = indDst + 1;
			}

			//3.
			connect(&processTry, &QProcess::readyReadStandardOutput, this, &MyMainWindow::processTry_readyReadStandardOutput);
			connect(&processTry, &QProcess::errorOccurred, this, &MyMainWindow::processTry_errorOccurred);
			connect(&processTry, static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished), this, &MyMainWindow::processTry_finished);

			processTry.setProgram(processPath);
			processTry.setArguments(listParam);
			processTry.start();
			QMessageBox::information(this, "Infomation", "Please wait for the process termination before other actions!");
			//void tryProcess(int nLoop, double x, double y, double z)
			//{
			//	while (--nLoop > 0)
			//	{
			//		double sum = 0.;
			//		int k = 100000000;
			//		while (--k > 0) sum += x * y * z;
			//		if (0) { cout << endl; cout << "cout: current progress and result: " << nLoop << "\t" << sum; }
			//		else if (0) { printf("\n"); printf("prinft: Currnet progress and result: %d\t%lf", nLoop, sum); }
			//		else if (1) { cout << endl; printf("cout + printf: currnet progress and result: %d\t%lf", nLoop, sum); }
			//	}
			//}
			//main:tryProcess(std::atoi(argv[1]), std::atof(argv[2]), std::atof(argv[3]), std::atof(argv[4]));
		}

		void processTry_readyReadStandardOutput()
		{
			yimageTryProcess += 20;
			if (yimageTryProcess >= imageTryProcess->height())
			{
				yimageTryProcess = 10;
				imageTryProcess->fill(QColor(225, 225, 225));
			}

			QPainter painter(imageTryProcess);
			painter.drawText(QPoint(10, yimageTryProcess), QString::fromLocal8Bit(processTry.readAllStandardOutput()));
			if (mutexActivateBG.try_lock())
			{
				pixmapActivateBG = QPixmap::fromImage(*imageTryProcess);
				mutexActivateBG.unlock();
			}		
		}
		void processTry_errorOccurred(QProcess::ProcessError error)
		{
			QString mes = QString("Error occured \n\n Error information: ");
			if (error == 0) mes += "FailedToStart";
			if (error == 1) mes += "Crashed";
			if (error == 2) mes += "Timedout";
			if (error == 3) mes += "WriteError";
			if (error == 4) mes += "ReadError";
			if (error == 5) mes += "UnknownError";
			QMessageBox::information(this, "Information", mes);
		}
		void processTry_finished(int exitCode, QProcess::ExitStatus exitStatus)
		{
			delete imageTryProcess;
			imageTryProcess = 0;
			disconnect(&processTry, &QProcess::readyReadStandardOutput, this, &MyMainWindow::processTry_readyReadStandardOutput);
			disconnect(&processTry, &QProcess::errorOccurred, this, &MyMainWindow::processTry_errorOccurred);
			disconnect(&processTry, static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished), this, &MyMainWindow::processTry_finished);
			QString mes = QString("exiCode: ") + aaa::num2string(exitCode).c_str() + "\n\n" + " exitStatus: " + (exitStatus == 0 ? "NormalExit" : "CrashExit");
			QMessageBox::information(this, "Information", mes);
		}

	public://3.相机打开项目
		std::thread threadOpenCamera;
		bool isSaveOpenCamera = false;
		bool isCloseOpenCamera = false;

		void actionOpenCamera_triggered()
		{
			DialogOpenCamera *dialogOpenCamera = new DialogOpenCamera(this);
			connect(dialogOpenCamera, &DialogOpenCamera::sendData, this, &MyMainWindow::dialogOpenCamera_closed);
			dialogOpenCamera->exec();
			delete dialogOpenCamera;
		}
		void dialogOpenCamera_closed(DialogOpenCamera::OpenCameraParams params)
		{
			//0.提取参数并初始化该模块参数
			if (threadOpenCamera.joinable()) { QMessageBox::information(this, "Information", "Please close camera first."); return; };
			isSaveOpenCamera = false;
			isCloseOpenCamera = false;
			abb::createDirectory(params.saveDir.toStdString());//创建目录

			//1.创建进程并执行(由于只需要一个子进程所以无需Timer辅助)
			threadOpenCamera = std::thread(threadOpenCamera_run, params, &pixmapActivateBG, &mutexActivateBG, &isSaveOpenCamera, &isCloseOpenCamera, statusbarMain);
		}
		void actionTakePhoto_triggered()
		{
			if (threadOpenCamera.joinable())
				isSaveOpenCamera = true;
			else QMessageBox::information(this, "Information", "Camera wasn't started");
		}
		void actionCloseCamera_triggered()
		{
			if (threadOpenCamera.joinable())
			{
				isCloseOpenCamera = true;
				threadOpenCamera.detach();
			}
			else QMessageBox::information(this, "Information", "Camera wasn't started");
		}
		static void threadOpenCamera_run(DialogOpenCamera::OpenCameraParams params, QPixmap *pixmap, std::mutex *mutex, bool *isSave, bool *isClose, QStatusBar *statusbar)
		{
			VideoCapture cap(params.cameraId);
			cap.set(CAP_PROP_FRAME_HEIGHT, params.frameRows);
			cap.set(CAP_PROP_FRAME_WIDTH, params.frameCols);

			Mat frame, frameRGB;
			while (cap.read(frame))
			{
				cvtColor(frame, frameRGB, COLOR_BGR2RGB);
				if (mutex->try_lock())
				{
					*pixmap = QPixmap::fromImage(QImage(frameRGB.data, frameRGB.cols, frameRGB.rows, QImage::Format_RGB888));
					mutex->unlock();
				}
				if (*isSave)
				{
					string savePath = QString(params.saveDir + "/" + QString::number(aaa_ns) + ".tif").toStdString();
					imwrite(savePath, frame); *isSave = false;
					statusbar->showMessage(QString("Save to: ") + savePath.c_str());//设置等待时间有异常后控制台输出异常
				}
				if (*isClose) { pixmap->fill(); return; }
			}
		}

	public://2.主界面项目

		//1.菜单栏及顶级菜单
		QMenuBar *menubarMain = new QMenuBar(this);
		QMenu *menuTools = new QMenu("Camera", menubarMain);
		QMenu *menuIODevice = new QMenu("IQDevice", menubarMain);

		//2.工具栏
		QToolBar *toolbarMain = new QToolBar("toobarmain", this);

		//3.工作区
		QLabel *labelWorkArea = new QLabel(this);
		QPixmap pixmapActivateBG = QPixmap::fromImage(QImage("./../data/nao.jpg"));//可独立背景项目
		QTimer *timerActivateBG = new QTimer(this);//可独立背景项目
		std::mutex mutexActivateBG;//可独立背景项目

		//4.状态栏
		QStatusBar *statusbarMain = new QStatusBar(this);

		//5.动作区
		QAction *actionActivateBG = new QAction(QIcon("./../data/window/background.ico"), "Activate Background", this);
		QAction *actionExit = new QAction(QIcon("./../data/window/close.ico"), "Application Exit", this);
		//5.1相机打开项目
		QAction *actionOpenCamera = new QAction(QIcon("./../data/window/camera.ico"), "Open Camera", this);
		QAction *actionTakePhoto = new QAction(QIcon("./../data/window/camera5.ico"), "Take Photo", this);
		QAction *actionCloseCamera = new QAction(QIcon("./../data/window/close1.ico"), "Close Camera", this);
		//5.5外设通信项目
		QAction *actionTryProcess = new QAction(QIcon("./../data/window/bird.ico"), "Try QProcess", this);
		QAction *actionRWSerialPort = new QAction(QIcon("./../data/window/bird2.ico"), "RW SerialPort", this);
		QAction *actionUseSqlite = new QAction(QIcon("./../data/window/bird4.ico"), "Use Sqlite", this);

		//6.功能实现
		MyMainWindow(QWidget *parent = 0) : QMainWindow(parent) { createMainUI(); createCentralUI(); }
		void createMainUI()
		{
			//0.基本设置
			this->setWindowTitle(typeid(this).name());
			this->setWindowIcon(QIcon("./../data/window/boss.ico"));
			//this->setWindowFlags(Qt::FramelessWindowHint);
			this->setMinimumSize(QSize(800, 600));

			//1.菜单栏及顶级菜单
			this->setMenuBar(menubarMain);
			menubarMain->addMenu(menuTools);
			menubarMain->addMenu(menuIODevice);
			//
			menuTools->addAction(actionActivateBG);
			menuTools->addAction(actionExit);
			//1.1相机打开项目
			menuTools->addSeparator();
			menuTools->addAction(actionOpenCamera);
			menuTools->addAction(actionTakePhoto);
			menuTools->addAction(actionCloseCamera);
			//1.5第三方进程项目
			menuIODevice->addAction(actionTryProcess);
			menuIODevice->addAction(actionRWSerialPort);
			menuIODevice->addAction(actionUseSqlite);

			//2.工具栏
			this->addToolBar(Qt::TopToolBarArea, toolbarMain);
			toolbarMain->setMovable(false);
			toolbarMain->addAction(actionActivateBG);
			toolbarMain->addAction(actionExit);
			//2.1相机打开项目
			toolbarMain->addSeparator();
			toolbarMain->addAction(actionOpenCamera);
			toolbarMain->addAction(actionTakePhoto);
			toolbarMain->addAction(actionCloseCamera);
			//2.5第三方进程项目
			toolbarMain->addSeparator();
			toolbarMain->addAction(actionTryProcess);
			toolbarMain->addAction(actionRWSerialPort);
			toolbarMain->addAction(actionUseSqlite);

			//3.工作区
			this->setCentralWidget(labelWorkArea);
			labelWorkArea->setScaledContents(true);
			connect(timerActivateBG, &QTimer::timeout, this, &MyMainWindow::timerActivateBG_timeout);

			//4.状态栏
			this->setStatusBar(statusbarMain);

			//5.动作关联
			connect(actionActivateBG, &QAction::triggered, this, &MyMainWindow::actionActivateBG_triggered);
			connect(actionExit, &QAction::triggered, &QApplication::exit);	
			//5.1相机打开项目
			connect(actionOpenCamera, &QAction::triggered, this, &MyMainWindow::actionOpenCamera_triggered);
			connect(actionTakePhoto, &QAction::triggered, this, &MyMainWindow::actionTakePhoto_triggered);
			connect(actionCloseCamera, &QAction::triggered, this, &MyMainWindow::actionCloseCamera_triggered);
			//5.5第三方进程项目
			connect(actionTryProcess, &QAction::triggered, this, &MyMainWindow::actionTryProcess_triggered);
			connect(actionRWSerialPort, &QAction::triggered, this, &MyMainWindow::actionRWSerialPort_triggered);
			connect(actionUseSqlite, &QAction::triggered, this, &MyMainWindow::actionUseSqlite_triggered);
		}
		void timerActivateBG_timeout()
		{
			if (mutexActivateBG.try_lock())
			{
				labelWorkArea->setPixmap(pixmapActivateBG);
				mutexActivateBG.unlock();
			}
		}
		void actionActivateBG_triggered()
		{
			if (timerActivateBG->isActive())
			{
				timerActivateBG->stop();
				pixmapActivateBG.fill();
				labelWorkArea->setPixmap(pixmapActivateBG);
				statusbarMain->showMessage("Background preview closed!", 2000);
			}
			else
			{
				timerActivateBG->start(100);
				pixmapActivateBG = QPixmap::fromImage(QImage("./../data/nao.jpg"));
				labelWorkArea->setPixmap(pixmapActivateBG);
				statusbarMain->showMessage("Background preview activated!", 2000);
			}
		}
	public: Q_OBJECT




		//////////////////////////////////////////////////////////
		//1.所有代码以追加的形式的添加，不对之前的代码做任改动，除在构造函数MyMainWindow中追加一句代码createCentralUI();外
		//2.移除了horizontalSpacer, 看看其它是否需要修改
		//3.之前的窗口是固定大小的, 现在改为可缩放, 但reset和Startup感觉高度太小, 此外, 缩放窗口时仅Current Pose在改变大小, 这两个问题需要解决下
		//4.Pose Setting 要检测鼠标连续按下事件, 表示连续发信号
	public:
		void pushButtonMoveRight_clickded() {}
		void pushButtonMoveLeft_clickded() {}
		void pushButtonMoveForward_clickded() {}
		void pushButtonMoveBackward_clickded() {}
		void pushButtonMoveUp_clickded() {}
		void pushButtonMoveDown_clickded() {}
		void pushButtonRotateClockwise_clickded() {}
		void pushButtonRotateAntiClockwise_clickded() {}
		void pushButtonReset_clickded() {}
		void PushButtonStartup_clickded() {}
	public:
		//1
		QVBoxLayout *vboxLayoutWorkArea = new QVBoxLayout(labelWorkArea);
		QTabWidget* tabWidgetScanAnalyse = new QTabWidget(labelWorkArea);
		QWidget* widgetScan = new QWidget(tabWidgetScanAnalyse);
		QWidget* widgetAnalyse = new QWidget(tabWidgetScanAnalyse);

		//2
		QGridLayout* gridLayoutWidgetScan = new QGridLayout(widgetScan);
		QGraphicsView* graphicsViewScan = new QGraphicsView(widgetScan);
		QGroupBox* groupBoxCurrentPose = new QGroupBox("Current Pose", widgetScan);
		QGroupBox* groupBoxPoseSetting = new QGroupBox("Pose Setting", widgetScan);
		QGroupBox* groupBoxScanSetting = new QGroupBox("Scan Setting", widgetScan);
		QGroupBox* groupBoxImplement = new QGroupBox("Implement", widgetScan);

		//3
		QGridLayout* gridLayoutGroupBoxCurrentPose = new QGridLayout(groupBoxCurrentPose);
		QLabel* labelXAxis = new QLabel("X Axis:", groupBoxCurrentPose);
		QLineEdit* lineEditXAxis = new QLineEdit("-1", groupBoxCurrentPose);
		QLabel* labelXmm = new QLabel("mm", groupBoxCurrentPose);
		QLabel* labelYAxis = new QLabel("Y Axis:", groupBoxCurrentPose);
		QLineEdit* lineEditYAxis = new QLineEdit("-1", groupBoxCurrentPose);
		QLabel* labelYmm = new QLabel("mm", groupBoxCurrentPose);
		QLabel* labelZAxis = new QLabel("Z Axis:", groupBoxCurrentPose);
		QLineEdit* lineEditZAxis = new QLineEdit("-1", groupBoxCurrentPose);
		QLabel* labelZmm = new QLabel("mm", groupBoxCurrentPose);
		QLabel* labelRotate = new QLabel("Rotate:", groupBoxCurrentPose);
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
		QLabel* labelStartPoint = new QLabel("Start Point:", groupBoxScanSetting);
		QLineEdit* lineEditStartPoint = new QLineEdit("0", groupBoxScanSetting);
		QLabel* labelStartPointMM = new QLabel("mm", groupBoxScanSetting);
		QLabel* labelEndPoint = new QLabel("End Point:", groupBoxScanSetting);
		QLineEdit* lineEditEndPoint = new QLineEdit("0", groupBoxScanSetting);
		QLabel* labelEndPointMM = new QLabel("mm", groupBoxScanSetting);
		QLabel* labelStep = new QLabel("Step", groupBoxScanSetting);
		QLabel* labelStepMM = new QLabel("mm", groupBoxScanSetting);
		QComboBox* comboBoxStep = new QComboBox(groupBoxScanSetting);

		//6
		QGridLayout* gridLayoutGroupBoxImplement = new QGridLayout(groupBoxImplement);
		QPushButton* pushButtonReset = new QPushButton("Reset", groupBoxImplement);
		QPushButton* PushButtonStartup = new QPushButton("Startup", groupBoxImplement);

		void createCentralUI()
		{
			//1
			vboxLayoutWorkArea->addWidget(tabWidgetScanAnalyse);
			tabWidgetScanAnalyse->addTab(widgetScan, "Scan");
			tabWidgetScanAnalyse->addTab(widgetAnalyse, "Analyse");
			tabWidgetScanAnalyse->setFont(QFont("", 20, QFont::Thin));

			//2
			gridLayoutWidgetScan->addWidget(graphicsViewScan, 0, 0, 1, 4);
			gridLayoutWidgetScan->addWidget(groupBoxCurrentPose, 1, 0, 1, 1);
			gridLayoutWidgetScan->addWidget(groupBoxPoseSetting, 1, 1, 1, 1);
			gridLayoutWidgetScan->addWidget(groupBoxScanSetting, 1, 2, 1, 1);
			gridLayoutWidgetScan->addWidget(groupBoxImplement, 1, 3, 1, 1);
			gridLayoutWidgetScan->setColumnStretch(0, 2);
			gridLayoutWidgetScan->setColumnStretch(1, 2);
			gridLayoutWidgetScan->setColumnStretch(2, 2);
			gridLayoutWidgetScan->setColumnStretch(3, 1);
			
			//3
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
			connect(pushButtonMoveRight, &QPushButton::clicked, this, &MyMainWindow::pushButtonMoveRight_clickded);
			connect(pushButtonMoveLeft, &QPushButton::clicked, this, &MyMainWindow::pushButtonMoveLeft_clickded);
			connect(pushButtonMoveForward, &QPushButton::clicked, this, &MyMainWindow::pushButtonMoveForward_clickded);
			connect(pushButtonMoveBackward, &QPushButton::clicked, this, &MyMainWindow::pushButtonMoveBackward_clickded);
			connect(pushButtonMoveUp, &QPushButton::clicked, this, &MyMainWindow::pushButtonMoveUp_clickded);
			connect(pushButtonMoveDown, &QPushButton::clicked, this, &MyMainWindow::pushButtonMoveDown_clickded);
			connect(pushButtonRotateClockwise, &QPushButton::clicked, this, &MyMainWindow::pushButtonRotateClockwise_clickded);
			connect(pushButtonRotateAntiClockwise, &QPushButton::clicked, this, &MyMainWindow::pushButtonRotateAntiClockwise_clickded);
			connect(pushButtonReset, &QPushButton::clicked, this, &MyMainWindow::pushButtonReset_clickded);
			connect(PushButtonStartup, &QPushButton::clicked, this, &MyMainWindow::PushButtonStartup_clickded);
		}

	};

}

#endif