#include <QSqlQuery>
#include <QSqlQueryModel>
#include <qwt_date_scale_draw.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_legenditem.h>
#include "WindowViewSingle.h"


class LegendItem : public QwtPlotLegendItem
{
public:
	LegendItem()
	{
		setRenderHint(QwtPlotItem::RenderAntialiased);

		QColor color(Qt::white);

		setTextPen(color);
#if 1
		setBorderPen(color);

		QColor c(Qt::gray);
		c.setAlpha(200);

		setBackgroundBrush(c);
#endif
	}
};

WindowViewSingle::WindowViewSingle(QWidget *parent, Qt::WindowFlags flags)
: QMainWindow(parent, flags)
{
	ui.setupUi(this);

	ui.dateEdit_3->setCalendarPopup(true);
	ui.dateEdit_4->setCalendarPopup(true);

	QSqlQueryModel* modelPhoneNumber = new QSqlQueryModel(this);
	ui.comboBox->setModel(modelPhoneNumber);
	connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(slotViewSingle()));

	QwtDateScaleDraw* xScaleDraw = new QwtDateScaleDraw();
	xScaleDraw->setDateFormat(QwtDate::Second, "hh:mm:ss\nddd dd MMM");
	ui.qwtPlot_3->setAxisScaleDraw(QwtPlot::xBottom, xScaleDraw);
	ui.qwtPlot_3->setAxisTitle(QwtPlot::yLeft, QStringLiteral("通话时长(秒)"));

	// canvas
	QwtPlotCanvas *canvas = new QwtPlotCanvas();
	canvas->setFocusIndicator(QwtPlotCanvas::CanvasFocusIndicator);
	canvas->setFocusPolicy(Qt::StrongFocus);
	canvas->setPalette(Qt::black);
	ui.qwtPlot_3->setCanvas(canvas);

//	ui.qwtPlot_3->setAutoReplot(false);

	// grid
	QwtPlotGrid *grid = new QwtPlotGrid;
	grid->enableXMin(true);
	grid->setMajorPen(Qt::gray, 0, Qt::DotLine);
	grid->setMinorPen(Qt::darkGray, 0, Qt::DotLine);
	grid->attach(ui.qwtPlot_3);

	// curve
	/*
	m_curveSelf = new QwtPlotCurve;
	m_curveSelf->setRenderHint(QwtPlotItem::RenderAntialiased);
	m_curveSelf->attach(ui.qwtPlot_3);
	m_curveOther = new QwtPlotCurve;
	m_curveOther->setRenderHint(QwtPlotItem::RenderAntialiased);
	m_curveOther->attach(ui.qwtPlot_3);
	*/
	m_curveAll = new QwtPlotCurve;
	m_curveAll->setRenderHint(QwtPlotItem::RenderAntialiased);
	m_curveAll->setPen(QColor("LightSalmon"), 2);
	m_curveAll->attach(ui.qwtPlot_3);

	slotCallLogChanged();
}

WindowViewSingle::~WindowViewSingle()
{
}

void WindowViewSingle::slotCallLogChanged()
{
	QString cmd;
	QSqlQuery query;
	query.exec("SELECT DISTINCT callnumber FROM CallLog");
	((QSqlQueryModel*)ui.comboBox->model())->setQuery(query);
	ui.comboBox->setCurrentIndex(0);

	QTextStream(&cmd) << "SELECT MIN(starttime) AS starttime FROM CallLog";
	query.exec(cmd);
	query.first();
	QVariant minDate = query.value("starttime");
	if (minDate.toUInt() == 0)
	{
		minDate = QDateTime::currentDateTime().toTime_t();
	}

	QTextStream(&cmd) << "SELECT MAX(starttime) AS starttime FROM CallLog";
	query.exec(cmd);
	query.first();
	QVariant maxDate = query.value("starttime");
	if (maxDate.toUInt() == 0)
	{
		maxDate = QDateTime::currentDateTime().toTime_t();
	}

	ui.dateEdit_3->setDate(QDateTime::fromTime_t(minDate.toUInt()).date());
	//	ui.dateEdit->setDateRange(QDateTime::fromTime_t(minDate.toUInt()).date(), QDateTime::fromTime_t(maxDate.toUInt()).date());
	ui.dateEdit_4->setDate(QDateTime::fromTime_t(maxDate.toUInt()).date());
	//	ui.dateEdit_2->setDateRange(QDateTime::fromTime_t(minDate.toUInt()).date(), QDateTime::fromTime_t(maxDate.toUInt()).date());
}

void WindowViewSingle::slotViewSingle()
{
	QString cmd;
	QTextStream(&cmd) << "SELECT * FROM CallLog WHERE starttime >= " 
		<< ui.dateEdit_3->dateTime().toTime_t()
		<< " AND starttime <= " << ui.dateEdit_4->dateTime().toTime_t()
		<< " AND callnumber = " << ui.comboBox->currentText();
	QSqlQuery query(cmd);

	QVector<QPointF> samples;
	while (query.next())
	{
		QPointF pos;
		pos.ry() = query.value("duration").toInt() / 1000;
		pos.rx() = query.value("starttime").toInt();
		samples.append(pos);
	}

//	m_curveSelf->setSamples();
//	m_curveOther->setSamples();
	m_curveAll->setSamples(samples); 
	m_curveAll->setVisible(true);
	ui.qwtPlot_3->replot();
}