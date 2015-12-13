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

//	setWindowState(Qt::WindowMaximized);

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

	ui.dateEdit->setCalendarPopup(true);
	ui.dateEdit_2->setCalendarPopup(true);
	connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(slotDateChanged()));

	slotCallLogChanged();
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

	connect(browser, SIGNAL(sqlDatachanged()), this, SLOT(slotCallLogChanged()));
	browser->show();
}

void ChartDialog::slotCallLogChanged()
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


	slotDateChanged();
}

void ChartDialog::slotDateChanged()
{
	m_timeChart->clear();
	m_countChart->clear();

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

		m_timeChart->addBar(phoneNumber, Qt::blue, time);
		m_countChart->addBar(phoneNumber, Qt::blue, count);
	}

	m_timeChart->replot();
	m_countChart->replot();

	return;
//	MyPlotLayout* timeLayout = (MyPlotLayout*)ui.qwtPlot->plotLayout();
//	MyPlotLayout* countLayout = (MyPlotLayout*)ui.qwtPlot_2->plotLayout();

	QWidget*  timeLayout = ui.qwtPlot->canvas();
	QWidget*  countLayout = ui.qwtPlot_2->canvas();


	QRectF timeYRect = timeLayout->rect();
	QRectF countYRect = countLayout->rect();

//	QRectF timeYRect = timeLayout->canvasRect();
//	QRectF countYRect = countLayout->canvasRect();

	int a = timeYRect.x();
	int a1 = countYRect.x();

	int w = timeYRect.width();
	int w1 = countYRect.width();

	int m;
	/*
	if (timeYRect.width() == countYRect.width())
	{
		countYRect.setWidth(10);
		timeYRect.setWidth(10);
		countLayout->mySetScaleRect(QwtPlot::yLeft, countYRect);
		timeLayout->mySetScaleRect(QwtPlot::yLeft, timeYRect);
	}
	else if (timeYRect.width() < countYRect.width())
	{
		timeYRect.setWidth(countYRect.width());
		timeLayout->mySetScaleRect(QwtPlot::yLeft, timeYRect);
	}
	*/
//	m_timeChart->replot();
//	m_countChart->replot();

//	ui.qwtPlot->repaint();
//	ui.qwtPlot_2->repaint();
}
