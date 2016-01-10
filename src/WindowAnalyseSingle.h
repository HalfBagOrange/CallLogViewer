#ifndef WINDOW_ANALYSE_SINGLE_H
#define WINDOW_ANALYSE_SINGLE_H

#include <QMainWindow>
#include <QMenu>
#include "CallLogPlot.h"
#include "ui_WindowAnalyseSingle.h"

class WindowAnalyseSingle : public QMainWindow
{
	Q_OBJECT
public:
	WindowAnalyseSingle(QWidget *parent = 0, Qt::WindowFlags flags = 0);
	~WindowAnalyseSingle();

public slots:
	void slotCallLogChanged();
	void slotDateTimeChanged();

	void slotTrialTimeout();
	void slotSuccessRegister();

private:
	Ui::WindowAnalyseSingle ui;
	CallLogPlot* m_timePlot;
	CallLogPlot* m_countPlot;
};

#endif