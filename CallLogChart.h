#ifndef CALL_LOG_CHART_H
#define CALL_LOG_CHART_H

#include <QMainWindow>
#include "ui_CallLogChart.h"

class BarChart;

class CallLogChart : public QMainWindow
{
	Q_OBJECT
public:
	CallLogChart(QWidget *parent = 0, Qt::WindowFlags flags = 0);
	~CallLogChart();

public slots:
	void slotDateTimeChanged();
	void slotImportFromWeb();
	void slotCallLogTable();
	void slotCallLogChanged();

private:
	void setupFileMenu();
	void setupToolMenu();
	void setupCallTimePlot();
	void setupCallNumberPlot();

private:
	Ui::CallLogChart ui;
	BarChart* m_timeBarChart;
	BarChart* m_numberBarChart;
};

#endif