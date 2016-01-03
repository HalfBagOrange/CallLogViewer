#include <qwt_plot_layout.h>
#include <qwt_column_symbol.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_renderer.h>
#include <qwt_plot_barchart.h>
#include <qwt_scale_draw.h>
#include <qwt_plot_barchart.h>
#include "CallLogPlot.h"

class BarScaleDrawX : public QwtScaleDraw
{
public:
	BarScaleDrawX()
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

	void addLabel(const QString& label)
	{
		m_labels += label;
	}

	void clear()
	{
		m_labels.clear();
	}

private:
    QStringList m_labels;
};

class BarScaleDrawY : public QwtScaleDraw
{
public:
	BarScaleDrawY() :QwtScaleDraw()
	{
		setTickLength(QwtScaleDiv::MediumTick, 4);
	}
	~BarScaleDrawY(){}

	virtual QwtText label(double value) const
	{
		return QwtText(QString::asprintf("% 5d", (int)value));
	}
};

class BarChart : public QwtPlotBarChart
{
public:
	BarChart(const QString & title = QString::null)
		: QwtPlotBarChart(title)
    {
		setLegendMode(QwtPlotBarChart::LegendBarTitles);
		setLegendIconSize(QSize(0, 0));
        setLayoutPolicy( AutoAdjustSamples );
        setLayoutHint(2.0); // minimum width for a single bar
        setSpacing(0); // spacing between bars
    }

	void addItem(double data, QString title, QColor color)
    {
        m_colors += color;
		m_titles += title;
		m_samples += data;
        //itemChanged();
    }

	void clear()
	{
		m_colors.clear();
		m_titles.clear();
		m_samples.clear();
	}

	void updateData()
	{
		setSamples(m_samples);
	}

    virtual QwtColumnSymbol *specialSymbol(
        int index, const QPointF& ) const
    {
        QwtColumnSymbol *symbol = new QwtColumnSymbol( QwtColumnSymbol::Box );
        symbol->setLineWidth( 2 );
        symbol->setFrameStyle( QwtColumnSymbol::Raised );

        QColor c( Qt::white );
        if ( index >= 0 && index < m_colors.size() )
            c = m_colors[ index ];

        symbol->setPalette( c );
        return symbol;
    }

    virtual QwtText barTitle( int sampleIndex ) const
    {
        QwtText title;
		if (sampleIndex >= 0 && sampleIndex < m_titles.size())
			title = m_titles[sampleIndex];
        return title;
    }

private:
    QList<QColor> m_colors;
	QList<QString> m_titles;
	QVector<double> m_samples;
};

CallLogPlot::CallLogPlot(QwtPlot* plot, QObject* parent)
	:QObject(parent)
	, m_plot(plot)
	, m_barScaleDrawX(NULL)
	, m_barCount(0)
{
	Q_ASSERT(m_plot);
	m_plot->setAutoReplot(false);

	// 设置图表颜色
	m_plot->setAutoFillBackground(true);
	m_plot->setPalette(QColor("Linen"));
	
	// 设置图表画布
	QwtPlotCanvas* canvas = new QwtPlotCanvas();
	canvas->setLineWidth(2);
	canvas->setFrameStyle(QFrame::Box | QFrame::Sunken);
	QPalette canvasPalette(QColor("Plum"));
	canvasPalette.setColor(QPalette::Foreground, QColor("Indigo"));
	canvas->setPalette(canvasPalette);
	m_plot->setCanvas(canvas);
	
	// 创建柱状图表
	m_barChart = new BarChart();
	m_barChart->attach(m_plot);
	m_barChart->setOrientation(Qt::Vertical);

	// 设置X坐标轴
	m_barScaleDrawX = new BarScaleDrawX();
	m_plot->setAxisMaxMajor(QwtPlot::xBottom, 0);
	m_plot->setAxisScaleDraw(QwtPlot::xBottom, m_barScaleDrawX);

	// 设置Y坐标轴
	m_barScaleDrawY = new BarScaleDrawY();
	m_plot->setAxisMaxMinor(QwtPlot::yLeft, 3);
	m_plot->setAxisScaleDraw(QwtPlot::yLeft, m_barScaleDrawY);

	// 设置布局
	m_plot->plotLayout()->setCanvasMargin(0);
	m_plot->replot();
}

void CallLogPlot::addBar(double data, QString title, QColor color)
{
	m_barChart->addItem(data, title, color);
	m_barScaleDrawX->addLabel(title);
	m_barCount++;
}

void CallLogPlot::clear()
{
	m_barChart->clear();
	m_barScaleDrawX->clear();
	m_barCount = 0;
}

void CallLogPlot::replot()
{
	m_barChart->updateData();
	m_plot->setAxisMaxMajor(QwtPlot::xBottom, m_barCount);
	m_plot->replot();
}

void CallLogPlot::exportChart(const QString &filename)
{
    QwtPlotRenderer renderer;
	renderer.exportTo(m_plot, filename);
}


