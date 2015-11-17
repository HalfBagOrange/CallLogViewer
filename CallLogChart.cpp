#include <qwt_plot_renderer.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_barchart.h>
#include <qwt_column_symbol.h>
#include <qwt_plot_layout.h>
#include <qwt_legend.h>
#include <qwt_scale_draw.h>
#include <qwt_plot_curve.h>
#include <QSqlQuery>
#include "CallLogChart.h"

class XScaleDraw : QwtScaleDraw
{
public:
	XScaleDraw() :QwtScaleDraw()
	{
		enableComponent(QwtScaleDraw::Backbone, false);
		enableComponent(QwtScaleDraw::Ticks, false);
		setLabelAlignment(Qt::AlignLeft);
		setLabelRotation(-90.0);
		setSpacing(0);
	}
	virtual QwtText label(double value) const
	{
		QwtText lbl;
		const int index = qRound(value);
		if ((value == (double)index) && index >= 0 && index < m_labels.size())
		{
			lbl = QwtText(m_labels[index], QwtText::TeXText);
			lbl.setLayoutAttribute(QwtText::MinimumLayout);
		}
		return lbl;
	}
	void addLabel(const QString& label){ m_labels += label; }
	void clear(){ m_labels.clear(); }
private:
	QStringList m_labels;
};

class BarChart : public QwtPlotBarChart
{
public:
	BarChart(const QwtText & title = QwtText()) :QwtPlotBarChart(title)
	{
		setLegendMode(QwtPlotBarChart::LegendBarTitles);
		setLegendIconSize(QSize(0, 0));
		setLayoutPolicy(AutoAdjustSamples);
		setLayoutHint(2.0);
		setSpacing(0);
	}
	void addItem(const QString &distro, const QColor &color, const double &sample)
	{
		m_colors += color;
		m_distros += distro;
		m_samples += sample;
	}
	void updateData()
	{
		setSamples(m_samples);
	}
	void clear()
	{
		m_colors.clear();
		m_distros.clear();
		m_samples.clear();
	}
	virtual QwtColumnSymbol *specialSymbol(
		int index, const QPointF&) const
	{
		QwtColumnSymbol *symbol = new QwtColumnSymbol(QwtColumnSymbol::Box);
		symbol->setLineWidth(2);
		symbol->setFrameStyle(QwtColumnSymbol::Raised);

		QColor c(Qt::white);
		if (index >= 0 && index < m_colors.size())
			c = m_colors[index];

		symbol->setPalette(c);
		return symbol;
	}
	virtual QwtText barTitle(int sampleIndex) const
	{
		QwtText title;
		if (sampleIndex >= 0 && sampleIndex < m_distros.size())
			title = m_distros[sampleIndex];
		return title;
	}
private:
	QList<QColor> m_colors;
	QList<QString> m_distros;
	QVector<double> m_samples;
};

CallLogChart::CallLogChart(QWidget *parent, Qt::WindowFlags flags)
	:QMainWindow(parent, flags)
{
	ui.setupUi(this);

	setupFileMenu();
	setupToolMenu();
	setupCallTimePlot();
	setupCallNumberPlot();

	ui.dateEdit->setCalendarPopup(true);
	ui.dateEdit_2->setCalendarPopup(true);
	connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(slotDateTimeChanged()));

	slotCallLogChanged();
}

CallLogChart::~CallLogChart()
{
}

// �ļ��˵���������WEB����...
void CallLogChart::setupFileMenu()
{
	QMenu* menu = ui.menuBar->addMenu(QStringLiteral("�ļ�"));
	QAction* action = menu->addAction(QStringLiteral("��WEB����....."));
	connect(action, SIGNAL(triggered()), this, SLOT(slotImportFromWeb()));
}

//���߲˵������������б�
void CallLogChart::setupToolMenu()
{
	QMenu* menu = ui.menuBar->addMenu(QStringLiteral("����"));
	QAction* action = menu->addAction(QStringLiteral("ͨ����¼�б�"));
	connect(action, SIGNAL(triggered()), this, SLOT(slotCallLogTable()));
}

void CallLogChart::setupCallTimePlot()
{
	// ����ͼ����ɫ
	ui.qwtPlot->setAutoFillBackground(true);
	//ui.qwtPlot->setAutoReplot(false);
	ui.qwtPlot->setPalette(QColor("Linen"));

	// ���û���
	QwtPlotCanvas* canvas = new QwtPlotCanvas();
	canvas->setLineWidth(2);
	canvas->setFrameStyle(QFrame::Box | QFrame::Sunken);
	//canvas->setBorderRadius(10);
	QPalette canvasPalette(QColor("Plum"));
	canvasPalette.setColor(QPalette::Foreground, QColor("Indigo"));
	canvas->setPalette(canvasPalette);
	ui.qwtPlot->setCanvas(canvas);

	// ����X��
	ui.qwtPlot->enableAxis(QwtPlot::xBottom, false);
	ui.qwtPlot->setAxisMaxMajor(QwtPlot::xBottom, 0);

	// ����Y��
	QwtText axisTitle;
	QFont axisTitleFont;
	axisTitleFont.setPixelSize(16);
	axisTitle.setText(QStringLiteral("ͨ��ʱ��(��)"));
	axisTitle.setFont(axisTitleFont);
	ui.qwtPlot->setAxisTitle(QwtPlot::yLeft, axisTitle);
	ui.qwtPlot->setAxisMaxMinor(QwtPlot::yLeft, 3);
	ui.qwtPlot->axisScaleDraw(QwtPlot::yLeft)->setTickLength(QwtScaleDiv::MediumTick, 4);

	m_timeBarChart = new BarChart();
	m_timeBarChart->attach(ui.qwtPlot);
}

void CallLogChart::setupCallNumberPlot()
{
	// ����ͼ����ɫ
	ui.qwtPlot_2->setAutoFillBackground(true);
	//ui.qwtPlot_2->setAutoReplot(false);
	ui.qwtPlot_2->setPalette(QColor("Linen"));

	// ���û���
	QwtPlotCanvas* canvas = new QwtPlotCanvas();
	canvas->setLineWidth(2);
	canvas->setFrameStyle(QFrame::Box | QFrame::Sunken);
	//canvas->setBorderRadius(10);
	QPalette canvasPalette(QColor("Plum"));
	canvasPalette.setColor(QPalette::Foreground, QColor("Indigo"));
	canvas->setPalette(canvasPalette);
	ui.qwtPlot_2->setCanvas(canvas);

	// ����X��
	XScaleDraw* xScale = new XScaleDraw();
	ui.qwtPlot_2->setAxisMaxMajor(QwtPlot::xBottom, 0);
	ui.qwtPlot_2->setAxisScaleDraw(QwtPlot::xBottom, (QwtScaleDraw*)xScale);

	// ����Y��
	QwtText axisTitle;
	QFont axisTitleFont;
	axisTitleFont.setPixelSize(16);
	axisTitle.setText(QStringLiteral("ͨ������(��)"));
	axisTitle.setFont(axisTitleFont);
	ui.qwtPlot_2->setAxisTitle(QwtPlot::yLeft, axisTitle);
	ui.qwtPlot_2->setAxisMaxMinor(QwtPlot::yLeft, 3);
	ui.qwtPlot_2->axisScaleDraw(QwtPlot::yLeft)->setTickLength(QwtScaleDiv::MediumTick, 4);

	m_numberBarChart = new BarChart();
	m_numberBarChart->attach(ui.qwtPlot_2);
}

void CallLogChart::slotCallLogChanged()
{
	QSqlQuery query;

	query.exec("SELECT MIN(callstarttime) AS callstarttime FROM CallLog");
	query.first();
	QVariant minDate = query.value("callstarttime");

	query.exec("SELECT MAX(callstarttime) AS callstarttime FROM CallLog");
	query.first();
	QVariant maxDate = query.value("callstarttime");

	ui.dateEdit->setDate(QDateTime::fromTime_t(minDate.toUInt()).date());
	ui.dateEdit->setDateRange(QDateTime::fromTime_t(minDate.toUInt()).date(), QDateTime::fromTime_t(maxDate.toUInt()).date());

	ui.dateEdit_2->setDate(QDateTime::fromTime_t(maxDate.toUInt()).date());
	ui.dateEdit_2->setDateRange(QDateTime::fromTime_t(minDate.toUInt()).date(), QDateTime::fromTime_t(maxDate.toUInt()).date());

	slotDateTimeChanged();
}

void CallLogChart::slotCallLogTable()
{
}

void CallLogChart::slotImportFromWeb()
{
}

void CallLogChart::slotDateTimeChanged()
{
	m_timeBarChart->clear();
	m_numberBarChart->clear();
	((XScaleDraw*)ui.qwtPlot_2->axisScaleDraw(QwtPlot::xBottom))->clear();

	QDateTime startDate = ui.dateEdit->dateTime();
	QDateTime endDate = ui.dateEdit_2->dateTime();

	//��ѯ�����еĵ绰����
	QString cmd;
	QTextStream(&cmd) << "SELECT DISTINCT phonenumber FROM CallLog WHERE callstarttime >= "
		<< startDate.toTime_t() << " AND callstarttime < " << endDate.addDays(1).toTime_t();
	QSqlQuery queryPhoneNumber(cmd);

	//ͳ��ͨ��ʱ�䡢ͨ������
	int phoneCount = 0;
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
		m_timeBarChart->addItem(phoneNumber, QColor("Blue"), time);
		m_numberBarChart->addItem(phoneNumber, QColor("Blue"), count);
		((XScaleDraw*)ui.qwtPlot_2->axisScaleDraw(QwtPlot::xBottom))->addLabel(phoneNumber);
		phoneCount++;
	}

	ui.qwtPlot->setAxisMaxMajor(QwtPlot::xBottom, phoneCount);
	ui.qwtPlot_2->setAxisMaxMajor(QwtPlot::xBottom, phoneCount);

	m_timeBarChart->updateData();
	m_numberBarChart->updateData();

	ui.qwtPlot->replot();
	ui.qwtPlot_2->replot();
}
