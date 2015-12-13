#include "barchart.h"
#include <qwt_plot_renderer.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_barchart.h>
#include <qwt_column_symbol.h>
#include <qwt_plot_layout.h>
#include <qwt_legend.h>
#include <qwt_scale_draw.h>

#include <QSqlDatabase>  
#include <QSqlQuery>
#include <QDebug>
#include <QSqlTableModel>

#include "ChartDialog.h"

class DistroScaleDraw: public QwtScaleDraw
{
public:
    DistroScaleDraw( Qt::Orientation orientation)
    {
//        setTickLength( QwtScaleDiv::MinorTick, 0 );
//        setTickLength( QwtScaleDiv::MediumTick, 0 );
//        setTickLength( QwtScaleDiv::MajorTick, 4 );

        enableComponent( QwtScaleDraw::Backbone, false );
		enableComponent( QwtScaleDraw::Ticks, false );
//		enableComponent(QwtScaleDraw::Labels, false);

		setLabelAlignment(Qt::AlignLeft);
		setLabelRotation(-90.0);
		setSpacing(0);
		
    }
    virtual QwtText label( double value ) const
    {
        QwtText lbl;
        const int index = qRound( value );
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

class YScaleDraw : public QwtScaleDraw
{
public:
	YScaleDraw() :QwtScaleDraw(){}
	~YScaleDraw(){}

	virtual QwtText label(double value) const
	{
		
		char a[6] = "";
		qsnprintf(a, 6, "% 5d", (int)value);
		QString str(a);
		QwtText lbl(str);
		return lbl;
	}
};

class DistroChartItem: public QwtPlotBarChart
{
public:
    DistroChartItem()
        //:QwtPlotBarChart( "Page Hits" )
    {
		setLegendMode(QwtPlotBarChart::LegendBarTitles);
        //setLegendIconSize( QSize( 10, 14 ) );
		setLegendIconSize(QSize(0, 0));
        setLayoutPolicy( AutoAdjustSamples );
        setLayoutHint( 2.0 ); // minimum width for a single bar
        setSpacing( 0 ); // spacing between bars
    }

	void addItem(const QString &distro, const QColor &color, const double &sample)
    {
        m_colors += color;
        m_distros += distro;
		m_samples += sample;
        itemChanged();
    }

	void clear()
	{
		m_colors.clear();
		m_distros.clear();
		m_samples.clear();
	}

	void updateData()
	{
		setSamples(m_samples);
	}

    virtual QwtColumnSymbol *specialSymbol(
        int index, const QPointF& ) const
    {
        // we want to have individual colors for each bar
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
        if ( sampleIndex >= 0 && sampleIndex < m_distros.size() )
            title = m_distros[ sampleIndex ];
        return title;
    }

private:
    QList<QColor> m_colors;
    QList<QString> m_distros;
	QVector<double> m_samples;
};

BarChart::BarChart(QObject *parent)
	:QObject(parent)
	, m_plot(NULL)
	, m_barChartItem(NULL)
	, m_barScaleX(NULL)
	, m_count(0)
{
}

void BarChart::setPlot(QwtPlot* plot)
{
	Q_ASSERT(NULL == m_plot && plot);
	m_plot = plot;

	// 设置图标颜色
	m_plot->setAutoFillBackground(true);
	m_plot->setAutoReplot(false);
	m_plot->setPalette(QColor("Linen"));

	// 设置画布
	QwtPlotCanvas* canvas = new QwtPlotCanvas();
	canvas->setLineWidth(2);
	canvas->setFrameStyle(QFrame::Box | QFrame::Sunken);
	//canvas->setBorderRadius(10);
	QPalette canvasPalette(QColor("Plum"));
	canvasPalette.setColor(QPalette::Foreground, QColor("Indigo"));
	canvas->setPalette(canvasPalette);
	m_plot->setCanvas(canvas);

	m_barChartItem = new DistroChartItem();
    m_barChartItem->attach( m_plot );

//    m_plot->insertLegend( new QwtLegend() );

    setOrientation( 0 );

	MyPlotLayout* layout = new MyPlotLayout();
	m_plot->setPlotLayout((QwtPlotLayout*)layout);

    m_plot->setAutoReplot( false );
}

void BarChart::setOrientation( int o )
{
    const Qt::Orientation orientation =
        ( o == 0 ) ? Qt::Vertical : Qt::Horizontal;

    int axis1 = QwtPlot::xBottom;
    int axis2 = QwtPlot::yLeft;

    if ( orientation == Qt::Horizontal )
        qSwap( axis1, axis2 );

    m_barChartItem->setOrientation( orientation );

//    m_plot->setAxisTitle( axis1, "Distros" );
//	m_plot->setAxisMaxMinor(axis1, 0);
	m_barScaleX = new DistroScaleDraw(orientation);

	{
//	QwtScaleDraw *scaleDraw = new QwtScaleDraw();
//	m_plot->setAxisMaxMajor(axis1, 1);
		m_plot->setAxisMaxMajor(axis1, 0);
//	m_barScaleX->setTickLength(QwtScaleDiv::MediumTick, 4);
	m_plot->setAxisScaleDraw(axis1, m_barScaleX);
//	m_plot->enableAxis(QwtPlot::xBottom, false);
	}
//    m_plot->setAxisTitle( axis2, "Hits per day ( HPD )" );
	m_plot->setAxisMaxMinor(axis2, 3);

	{
		QwtScaleDraw *scaleDraw = (YScaleDraw*)new YScaleDraw();
    scaleDraw->setTickLength( QwtScaleDiv::MediumTick, 4 );
    m_plot->setAxisScaleDraw( axis2, scaleDraw );
	//m_plot->setAxisTitle(axis2, QStringLiteral("通话时间(秒)"));
	}

    m_plot->plotLayout()->setCanvasMargin( 0 );
    m_plot->replot();
}

void BarChart::addBar(const QString &distro, const QColor &color, double const& data)
{
	m_barChartItem->addItem(distro, color, data);
	m_barScaleX->addLabel(distro);
	m_count++;
}

void BarChart::clear()
{
	m_barChartItem->clear();
	m_barScaleX->clear();
	m_count = 0;
}

void BarChart::replot()
{
	m_plot->setAxisMaxMajor(QwtPlot::xBottom, m_count);
	m_barChartItem->updateData();
	m_plot->replot();
}

void BarChart::exportChart(const QString &filename)
{
    QwtPlotRenderer renderer;
	renderer.exportTo(m_plot, filename);
}


