#include <qwt_plot_renderer.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_barchart.h>
#include <qwt_column_symbol.h>
#include <qwt_plot_layout.h>
#include <qwt_legend.h>
#include <qwt_scale_draw.h>
#include <QSqlQuery>
#include "ChartDialog.h"

ChartDialog::ChartDialog(QWidget *parent, Qt::WindowFlags flags)
	:QMainWindow(parent, flags)
{
	ui.setupUi(this);

	/*
	ui.comboBox_2->insertItem(0, QStringLiteral("Ìì"), "day");
	ui.comboBox_2->insertItem(0, QStringLiteral("ÖÜ"), "week");
	ui.comboBox_2->insertItem(0, QStringLiteral("ÔÂ"), "month");
	*/

	m_timeChart = new BarChart(this);
	m_timeChart->setPlot(ui.qwtPlot);

	m_countChart = new BarChart(this);
	m_countChart->setPlot(ui.qwtPlot_2);

	/*
	m_sqlTableModel = new QSqlTableModel(this);
	QSqlQuery query;
	query.exec("SELECT DISTINCT phonenumber FROM CallLog");
	m_sqlTableModel->setQuery(query);
	ui.comboBox->setModel(m_sqlTableModel);

	connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(statsButtonClicked()));
	*/
//	query.exec("SELECT callstarttime FROM CallLog ORDER BY callstarttime");
	QSqlQuery query;

	query.exec("SELECT MIN(callstarttime) AS callstarttime FROM CallLog");
	query.first();
	QVariant min = query.value("callstarttime");

	query.exec("SELECT MAX(callstarttime) AS callstarttime FROM CallLog");
	query.first();
	QVariant max = query.value("callstarttime");

	ui.dateEdit->setCalendarPopup(true);
	ui.dateEdit->setDate(QDateTime::fromTime_t(min.toUInt()).date());
//	ui.dateEdit->setDateRange(QDateTime::fromTime_t(min.toUInt()).date(), QDateTime::fromTime_t(max.toUInt()).date());

	ui.dateEdit_2->setCalendarPopup(true);
	ui.dateEdit_2->setDate(QDateTime::fromTime_t(max.toUInt()).date());
//	ui.dateEdit_2->setDateRange(QDateTime::fromTime_t(min.toUInt()).date(), QDateTime::fromTime_t(max.toUInt()).date());

//	connect(ui.dateEdit, SIGNAL(editingFinished()), this, SLOT(dateTimeChanged()));
//	connect(ui.dateEdit_2, SIGNAL(editingFinished()), this, SLOT(dateChanged()));
}

ChartDialog::~ChartDialog()
{
}

void ChartDialog::slotDateChanged()
{
	QDateTime startDate = ui.dateEdit->dateTime();
	QDateTime endDate = ui.dateEdit_2->dateTime();

	QString cmd;
	QTextStream(&cmd) << "SELECT DISTINCT phonenumber FROM CallLog WHERE callstarttime >= "
		<< startDate.toTime_t() << " AND callstarttime < " << startDate.addDays(1).toTime_t();

	QSqlQuery queryPhoneNumber(cmd);
	while (queryPhoneNumber.next())
	{
		QString phoneNumber = queryPhoneNumber.value("phonenumber").toString();

		QTextStream(&cmd) << "SELECT * FROM CallLog WHERE callstarttime >= "
			<< startDate.toTime_t() << " AND callstarttime < " << endDate.toTime_t()
			<< " AND phonenumber = " << phoneNumber;

		QSqlQuery queryCallLog(cmd);
		int count = queryCallLog.size();
		int time = 0;
		while (queryCallLog.next())
		{
			time += queryCallLog.value("callduration").toInt();
		}
	}
}

/*
void ChartDialog::statsButtonClicked()
{
	QString number = ui.comboBox->currentText();
	QDateTime startDate = ui.dateEdit->dateTime();
	QDateTime endDate = ui.dateEdit_2->dateTime();
	QVariant type = ui.comboBox_2->currentData();

	m_barChart->clear();
	for (; startDate <= endDate; startDate = startDate.addDays(1))
	{
		QString cmd;
		QTextStream(&cmd) << "SELECT * FROM CallLog WHERE callstarttime >= "
			<< startDate.toTime_t() << " AND callstarttime < " << startDate.addDays(1).toTime_t()
		<< " AND phonenumber = " << number;
		QSqlQuery query(cmd);
		query.last();
		int count = query.at() + 1;

		m_barChart->addBar(startDate.toString("yy/MM/dd"), Qt::blue, count > 0 ? count : 0);
	}

	m_barChart->replot();
}
*/