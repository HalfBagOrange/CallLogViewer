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
	:QDialog(parent, flags)
{
	ui.setupUi(this);

	ui.comboBox_2->insertItem(0, QStringLiteral("Ìì"), "day");
	ui.comboBox_2->insertItem(0, QStringLiteral("ÖÜ"), "week");
	ui.comboBox_2->insertItem(0, QStringLiteral("ÔÂ"), "month");

	m_barChart = new BarChart(this);
	m_barChart->setPlot(ui.qwtPlot);

	m_sqlTableModel = new QSqlTableModel(this);
	QSqlQuery query;
	query.exec("SELECT DISTINCT phonenumber FROM CallLog");
	m_sqlTableModel->setQuery(query);
	ui.comboBox->setModel(m_sqlTableModel);

	connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(statsButtonClicked()));

//	query.exec("SELECT callstarttime FROM CallLog ORDER BY callstarttime");
	query.exec("SELECT MIN(callstarttime) AS callstarttime FROM CallLog");
	query.first();
	QVariant min = query.value("callstarttime");
	query.exec("SELECT MAX(callstarttime) AS callstarttime FROM CallLog");
	query.first();
//	query.last();
	QVariant max = query.value("callstarttime");

	ui.dateEdit->setDate(QDateTime::fromTime_t(min.toUInt()).date());
	ui.dateEdit->setDateRange(QDateTime::fromTime_t(min.toUInt()).date(), QDateTime::fromTime_t(max.toUInt()).date());

	ui.dateEdit_2->setDate(QDateTime::fromTime_t(max.toUInt()).date());
	ui.dateEdit_2->setDateRange(QDateTime::fromTime_t(min.toUInt()).date(), QDateTime::fromTime_t(max.toUInt()).date());
}

ChartDialog::~ChartDialog()
{
}

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
