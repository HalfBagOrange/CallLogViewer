#include <qwt_plot_renderer.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_barchart.h>
#include <qwt_column_symbol.h>
#include <qwt_plot_layout.h>
#include <qwt_legend.h>
#include <qwt_scale_draw.h>
#include <QSqlQuery>
#include "ChartDialog.h"

#include "mainwindow.h"
#include "CallLogViewer.h"

#include <qwt_plot_curve.h>

ChartDialog::ChartDialog(QWidget *parent, Qt::WindowFlags flags)
	:QMainWindow(parent, flags)
{
	ui.setupUi(this);

	setWindowState(Qt::WindowMaximized);

	QMenu* fileMenu = ui.menuBar->addMenu(QStringLiteral("文件"));
	QAction* webImportAct = fileMenu->addAction(QStringLiteral("从WEB导入....."));
	connect(webImportAct, SIGNAL(triggered()), this, SLOT(slotImportFromWeb()));

	QMenu* toolMenu = ui.menuBar->addMenu(QStringLiteral("工具"));
	QAction* chartAct = toolMenu->addAction(QStringLiteral("数据列表"));
	connect(chartAct, SIGNAL(triggered()), this, SLOT(slotDateList()));

	QwtText axisTitle;
	QFont axisTitleFont;
	axisTitleFont.setPixelSize(16);

	axisTitle.setText(QStringLiteral("通话时间(秒)"));
	axisTitle.setFont(axisTitleFont);

	m_timeChart = new BarChart(this);
	m_timeChart->setPlot(ui.qwtPlot);
	ui.qwtPlot->enableAxis(QwtPlot::xBottom, false);
	ui.qwtPlot->setAxisTitle(QwtPlot::yLeft, axisTitle);

	axisTitle.setText(QStringLiteral("通话次数(次)"));
	axisTitle.setFont(axisTitleFont);

	m_countChart = new BarChart(this);
	m_countChart->setPlot(ui.qwtPlot_2);
	ui.qwtPlot_2->setAxisTitle(QwtPlot::yLeft, axisTitle);

	QSqlQuery query;

	query.exec("SELECT MIN(callstarttime) AS callstarttime FROM CallLog");
	query.first();
	QVariant minDate = query.value("callstarttime");

	query.exec("SELECT MAX(callstarttime) AS callstarttime FROM CallLog");
	query.first();
	QVariant maxDate = query.value("callstarttime");

	ui.dateEdit->setCalendarPopup(true);
	ui.dateEdit->setDate(QDateTime::fromTime_t(minDate.toUInt()).date());
	ui.dateEdit->setDateRange(QDateTime::fromTime_t(minDate.toUInt()).date(), QDateTime::fromTime_t(maxDate.toUInt()).date());

	ui.dateEdit_2->setCalendarPopup(true);
	ui.dateEdit_2->setDate(QDateTime::fromTime_t(maxDate.toUInt()).date());
	ui.dateEdit_2->setDateRange(QDateTime::fromTime_t(minDate.toUInt()).date(), QDateTime::fromTime_t(maxDate.toUInt()).date());
	
	connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(slotDateChanged()));
	slotDateChanged();
}

ChartDialog::~ChartDialog()
{
}

void ChartDialog::slotDateList()
{
	CallLogViewer* viewer = new CallLogViewer();
	viewer->show();
}

void ChartDialog::slotImportFromWeb()
{
	QUrl url = QUrl(QStringLiteral("http://www.189.cn/"));
	MainWindow* browser = new MainWindow(url);

	connect(browser, SIGNAL(sqlDatachanged()), this, SLOT(updateSqlModel()));
	browser->show();
}

void ChartDialog::slotDateChanged()
{
	m_timeChart->clear();
	m_countChart->clear();

	QDateTime startDate = ui.dateEdit->dateTime();
	QDateTime endDate = ui.dateEdit_2->dateTime();

	QString cmd;
	QTextStream(&cmd) << "SELECT DISTINCT phonenumber FROM CallLog WHERE callstarttime >= "
		<< startDate.toTime_t() << " AND callstarttime < " << endDate.addDays(1).toTime_t();

	QSqlQuery queryPhoneNumber(cmd);
	while (queryPhoneNumber.next())
	{
		QString phoneNumber = queryPhoneNumber.value("phonenumber").toString();

		cmd.clear();
		QTextStream(&cmd) << "SELECT * FROM CallLog WHERE callstarttime >= "
			<< startDate.toTime_t() << " AND callstarttime < " << endDate.addDays(1).toTime_t()
			<< " AND phonenumber = " << phoneNumber;

		QSqlQuery queryCallLog(cmd);
		int count = 0;
		int time = 0;
		while (queryCallLog.next())
		{
			count++;
			time += queryCallLog.value("callduration").toInt()/1000;
		}

		m_timeChart->addBar(phoneNumber, Qt::blue, time);
		m_countChart->addBar(phoneNumber, Qt::blue, count);
	}

	m_timeChart->replot();
	m_countChart->replot();
}
