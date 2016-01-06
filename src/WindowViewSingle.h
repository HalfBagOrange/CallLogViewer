#ifndef WINDOW_VIEW_SINGLE_H
#define WINDOW_VIEW_SINGLE_H

#include <QMainWindow>
#include "ui_WindowViewSingle.h"

class QwtPlotCurve;

class WindowViewSingle :public QMainWindow
{
	Q_OBJECT
public:
	WindowViewSingle(QWidget *parent = 0, Qt::WindowFlags flags = 0);
	~WindowViewSingle();

public slots:
	void slotCallLogChanged();
	void slotViewSingle();

private:
	Ui::WindowViewSingle ui;

	QwtPlotCurve* m_curveSelf;
	QwtPlotCurve* m_curveOther;
	QwtPlotCurve* m_curveAll;
};

#endif