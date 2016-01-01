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
#include "HelpDialog.h"
#include "Check.h"

ChartDialog::ChartDialog(QWidget *parent, Qt::WindowFlags flags)
	:QMainWindow(parent, flags)
{
	ui.setupUi(this);

	setWindowState(Qt::WindowMaximized);

	QMenu* fileMenu = ui.menuBar->addMenu(QStringLiteral("�ļ�"));
	QAction* webImportAct = fileMenu->addAction(QStringLiteral("��WEB����....."));
	connect(webImportAct, SIGNAL(triggered()), this, SLOT(slotImportFromWeb()));

	QMenu* toolMenu = ui.menuBar->addMenu(QStringLiteral("����"));
	QAction* chartAct = toolMenu->addAction(QStringLiteral("�����б�"));
	connect(chartAct, SIGNAL(triggered()), this, SLOT(slotDateList()));

	QMenu* helpMenu = ui.menuBar->addMenu(QStringLiteral("����"));
	QAction* abortAct = helpMenu->addAction(QStringLiteral("�������"));
	connect(abortAct, SIGNAL(triggered()), this, SLOT(slotAbort()));

	QwtText axisTitle;
	QFont axisTitleFont;
	axisTitleFont.setPixelSize(16);

	axisTitle.setText(QStringLiteral("ͨ��ʱ��(��)"));
	axisTitle.setFont(axisTitleFont);

	m_timeChart = new BarChart(this);
	m_timeChart->setPlot(ui.qwtPlot);
	ui.qwtPlot->enableAxis(QwtPlot::xBottom, false);
	ui.qwtPlot->setAxisTitle(QwtPlot::yLeft, axisTitle);

	axisTitle.setText(QStringLiteral("ͨ������(��)"));
	axisTitle.setFont(axisTitleFont);

	m_countChart = new BarChart(this);
	m_countChart->setPlot(ui.qwtPlot_2);
	ui.qwtPlot_2->setAxisTitle(QwtPlot::yLeft, axisTitle);

	ui.dateEdit->setCalendarPopup(true);
	ui.dateEdit_2->setCalendarPopup(true);
	connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(slotDateChanged()));

	m_sqlTableModel = new QMySqlTableModel(this);
	ui.tableView->setModel(m_sqlTableModel);

	m_comSqlModel = new QSqlQueryModel(this);
	ui.comboBox->setModel(m_comSqlModel);

	connect(ui.comboBox, SIGNAL(activated(const QString &)), this, SLOT(slotPhoneNumberChanged(const QString &)));

	m_trialTimeout = false;

	connect(CCheck::instance(), SIGNAL(signalTrialTimeout()), this, SLOT(slotTrialTimeout()));
	connect(CCheck::instance(), SIGNAL(signalSuccessRegisted()), this, SLOT(slotSuccessRegisted()));

	slotCallLogChanged();
}

ChartDialog::~ChartDialog()
{
}

void ChartDialog::slotTrialTimeout()
{
	if (m_trialTimeout)
	{
		return;
	}
	ui.pushButton->setEnabled(false);
	m_trialTimeout = true;
	QMessageBox::about(this, QStringLiteral("����"), QStringLiteral("���������ڣ�������ע��"));
}

void ChartDialog::slotSuccessRegisted()
{
	ui.pushButton->setEnabled(true);
	m_trialTimeout = false;
}

void ChartDialog::slotAbort()
{
	HelpDialog* help = new HelpDialog(this);
	help->show();
}

void ChartDialog::slotDateList()
{
	if (m_trialTimeout)
	{
		return;
	}

	CallLogViewer* viewer = new CallLogViewer();
	viewer->show();
}

void ChartDialog::slotImportFromWeb()
{
	if (m_trialTimeout)
	{
		return;
	}

	QUrl url;// = QUrl(QStringLiteral("http://www.189.cn/"));
	MainWindow* browser = new MainWindow(url);

	connect(browser, SIGNAL(sqlDatachanged()), this, SLOT(slotCallLogChanged()));
	browser->show();
}

void ChartDialog::slotCallLogChanged()
{
	if (m_trialTimeout)
	{
		return;
	}

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

	m_sqlTableModel->setTable("CallLog");
	m_sqlTableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
	m_sqlTableModel->select();

	m_sqlTableModel->setHeaderData(0, Qt::Horizontal, QVariant(QStringLiteral("�Է�����")));
	m_sqlTableModel->setHeaderData(1, Qt::Horizontal, QVariant(QStringLiteral("ͨ������")));
	m_sqlTableModel->setHeaderData(2, Qt::Horizontal, QVariant(QStringLiteral("��ʼʱ��")));
	m_sqlTableModel->setHeaderData(3, Qt::Horizontal, QVariant(QStringLiteral("ͨ��ʱ��")));
	m_sqlTableModel->setHeaderData(4, Qt::Horizontal, QVariant(QStringLiteral("ͨ����")));
	m_sqlTableModel->setHeaderData(5, Qt::Horizontal, QVariant(QStringLiteral("�Է�ͨ�����ڵ�")));
	m_sqlTableModel->setHeaderData(6, Qt::Horizontal, QVariant(QStringLiteral("ͨ������LAC")));
	m_sqlTableModel->setHeaderData(7, Qt::Horizontal, QVariant(QStringLiteral("ͨ������CELLID")));

	query.exec("SELECT DISTINCT callnumber FROM CallLog");
	m_comSqlModel->setQuery(query);

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
}

void ChartDialog::slotPhoneNumberChanged(const QString & text)
{
	QString cmd;
	QTextStream(&cmd) << "callnumber = " << text;
	m_sqlTableModel->setFilter(cmd);
}
