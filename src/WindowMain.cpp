#include <QSqlQuery>
#include <QMessageBox>

#include "WindowAnalyseSingle.h"
#include "WindowBrowser.h"
#include "WindowTable.h"
#include "ObjectRegister.h"
#include "DialogHelp.h"
#include "WindowMain.h"

WindowMain::WindowMain(QWidget *parent, Qt::WindowFlags flags)
	:QMainWindow(parent, flags)
{
	ui.setupUi(this);

	setWindowState(Qt::WindowMaximized);

	m_fileMenu = ui.menuBar->addMenu(QStringLiteral("文件"));
	QAction* webImportAct = m_fileMenu->addAction(QStringLiteral("从Html导入....."));
	connect(webImportAct, SIGNAL(triggered()), this, SLOT(slotImportFromHtml()));

	m_toolMenu = ui.menuBar->addMenu(QStringLiteral("工具"));
	QAction* viewSingleAct = m_toolMenu->addAction(QStringLiteral("单个号码通信视图"));
	connect(viewSingleAct, SIGNAL(triggered()), this, SLOT(slotViewSingle()));
	QAction* chartAct = m_toolMenu->addAction(QStringLiteral("数据列表"));
	connect(chartAct, SIGNAL(triggered()), this, SLOT(slotCallLogTable()));

	QMenu* helpMenu = ui.menuBar->addMenu(QStringLiteral("帮助"));
	QAction* abortAct = helpMenu->addAction(QStringLiteral("关于软件"));
	connect(abortAct, SIGNAL(triggered()), this, SLOT(slotAbout()));

	QwtText axisTitle;
	QFont axisTitleFont;
	axisTitleFont.setPixelSize(16);

	axisTitle.setText(QStringLiteral("通话时间(秒)"));
	axisTitle.setFont(axisTitleFont);
	m_timePlot = new CallLogPlot(ui.qwtPlot, this);
	m_timePlot->getPlot()->enableAxis(QwtPlot::xBottom, false);
	m_timePlot->getPlot()->setAxisTitle(QwtPlot::yLeft, axisTitle);

	axisTitle.setText(QStringLiteral("通话次数(次)"));
	axisTitle.setFont(axisTitleFont);
	m_countPlot = new CallLogPlot(ui.qwtPlot_2, this);
	m_countPlot->getPlot()->setAxisTitle(QwtPlot::yLeft, axisTitle);

	ui.dateEdit->setCalendarPopup(true);
	ui.dateEdit_2->setCalendarPopup(true);

	connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(slotDateTimeChanged()));
	connect(ObjectRegister::instance(), SIGNAL(signalTrialTimeout()), this, SLOT(slotTrialTimeout()));
	connect(ObjectRegister::instance(), SIGNAL(signalSuccessRegister()), this, SLOT(slotSuccessRegister()));

	slotCallLogChanged();
}

WindowMain::~WindowMain()
{
}

void WindowMain::slotTrialTimeout()
{
	ui.dateEdit->setEnabled(false);
	ui.dateEdit_2->setEnabled(false);
	ui.pushButton->setEnabled(false);

	m_fileMenu->setEnabled(false);
	m_toolMenu->setEnabled(false);

	QMessageBox::about(this, QStringLiteral("警告"), QStringLiteral("超出试用期，请重新注册"));

	QList<QMainWindow *> windows = findChildren<QMainWindow *>();
	for (int i = 0; i < windows.size(); i++)
	{
		windows[i]->close();
	}
}

void WindowMain::slotSuccessRegister()
{
	ui.dateEdit->setEnabled(true);
	ui.dateEdit_2->setEnabled(true);
	ui.pushButton->setEnabled(true);

	m_fileMenu->setEnabled(true);
	m_toolMenu->setEnabled(true);
}

void WindowMain::slotImportFromHtml()
{
	WindowBrowser* browser = new WindowBrowser(this);
	connect(browser, SIGNAL(signalCallLogChanged()), this, SLOT(slotCallLogChanged()));
	browser->show();
}

void WindowMain::slotCallLogTable()
{
	WindowTable* table = new WindowTable(this);
	connect(this, SIGNAL(signalChallLogChanged()), table, SLOT(slotCallLogChanged()));
	table->show();
}

void WindowMain::slotAbout()
{
	DialogHelp* help = new DialogHelp(this);
	help->show();
}

void WindowMain::slotViewSingle()
{
	WindowAnalyseSingle* viewSingle = new WindowAnalyseSingle(this);
	connect(this, SIGNAL(signalChallLogChanged()), viewSingle, SLOT(slotCallLogChanged()));
	viewSingle->show();
	/*
	WindowViewSingle* viewSingle = new WindowViewSingle(this);
	connect(this, SIGNAL(signalChallLogChanged()), viewSingle, SLOT(slotCallLogChanged()));
	viewSingle->show();
	*/
}

void WindowMain::slotCallLogChanged()
{
	QSqlQuery query;
	query.exec("SELECT MIN(starttime) AS starttime FROM CallLog");
	query.first();
	QVariant minDate = query.value("starttime");
	if (minDate.toUInt() == 0)
	{
		minDate = QDateTime::currentDateTime().toTime_t();
	}

	query.exec("SELECT MAX(starttime) AS starttime FROM CallLog");
	query.first();
	QVariant maxDate = query.value("starttime");
	if (maxDate.toUInt() == 0)
	{
		maxDate = QDateTime::currentDateTime().toTime_t();
	}

	ui.dateEdit->setDate(QDateTime::fromTime_t(minDate.toUInt()).date());
//	ui.dateEdit->setDateRange(QDateTime::fromTime_t(minDate.toUInt()).date(), QDateTime::fromTime_t(maxDate.toUInt()).date());

	ui.dateEdit_2->setDate(QDateTime::fromTime_t(maxDate.toUInt()).date());
//	ui.dateEdit_2->setDateRange(QDateTime::fromTime_t(minDate.toUInt()).date(), QDateTime::fromTime_t(maxDate.toUInt()).date());

	slotDateTimeChanged();

	emit signalChallLogChanged();
}

void WindowMain::slotDateTimeChanged()
{
	m_timePlot->clear();
	m_countPlot->clear();

	QDateTime startDate = ui.dateEdit->dateTime();
	QDateTime endDate = ui.dateEdit_2->dateTime();

	QString cmd;
	QTextStream(&cmd) << "SELECT DISTINCT callnumber FROM CallLog WHERE starttime >= "
		<< startDate.toTime_t() << " AND starttime < " << endDate.addDays(1).toTime_t();

	QSqlQuery queryPhoneNumber(cmd);
	while (queryPhoneNumber.next())
	{
		QString phoneNumber = queryPhoneNumber.value("callnumber").toString();

		cmd.clear();
		QTextStream(&cmd) << "SELECT * FROM CallLog WHERE starttime >= "
			<< startDate.toTime_t() << " AND starttime < " << endDate.addDays(1).toTime_t()
			<< " AND callnumber = " << phoneNumber;

		QSqlQuery queryCallLog(cmd);

		int count = 0;
		int time = 0;
		while (queryCallLog.next())
		{
			count++;
			time += queryCallLog.value("duration").toInt()/1000;
		}

		m_timePlot->addBar(time, phoneNumber);
		m_countPlot->addBar(count, phoneNumber);
	}

	m_timePlot->replot();
	m_countPlot->replot();
}
