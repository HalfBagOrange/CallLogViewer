#ifndef CALL_LOG_PLOT_H
#define CALL_LOG_PLOT_H

#include <qwt_plot.h>

class BarChart;
class BarScaleDrawX;
class BarScaleDrawY;

class CallLogPlot : public QObject
{
    Q_OBJECT

public:
	CallLogPlot(QwtPlot* plot, QObject* parent = NULL);

	QwtPlot* getPlot(){ return m_plot; }
	void addBar(double data, QString title = QString::null, QColor color = Qt::blue);
	void clear();
	void replot();
	void exportChart(const QString & filename);

private:
	QwtPlot* m_plot;
    BarChart* m_barChart;
	BarScaleDrawX* m_barScaleDrawX;
	BarScaleDrawY* m_barScaleDrawY;

	int m_barCount;
};

#endif
