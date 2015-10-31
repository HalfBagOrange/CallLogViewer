#ifndef _BAR_CHART_H_
#define _BAR_CHART_H_

#include <qwt_plot.h>
#include <qstringlist.h>
#include <QDialog>

class DistroChartItem;
class DistroScaleDraw;

class BarChart: public QObject
{
    Q_OBJECT

public:
	BarChart(QObject* parent = NULL);
	void setPlot(QwtPlot* plot);

public Q_SLOTS:
	void setOrientation(int o);
	void addBar(const QString &distro, const QColor &color, double const& data);
	void clear();
	void replot();
	void exportChart(const QString &filename);

private:
	QwtPlot* m_plot;
    DistroChartItem* m_barChartItem;
	DistroScaleDraw* m_barScaleX;
	int m_count;
};

#endif
