#ifndef WINDOW_MAIN_H
#define WINDOW_MAIN_H

#include <QMainWindow>
#include <QMenu>
#include "CallLogPlot.h"
#include "ui_WindowMain.h"

class WindowMain : public QMainWindow
{
	Q_OBJECT
public:
	WindowMain(QWidget *parent = 0, Qt::WindowFlags flags = 0);
	~WindowMain();

signals:
	void signalChallLogChanged();

public slots:
	void slotCallLogChanged();
	void slotDateTimeChanged();

	void slotImportFromHtml();
	void slotCallLogTable();
	void slotAbout();
	void slotViewSingle();
	
	void slotTrialTimeout();
	void slotSuccessRegister();

private:
	Ui::WindowMain ui;

	QMenu* m_fileMenu;
	QMenu* m_toolMenu;
	CallLogPlot* m_timePlot;
	CallLogPlot* m_countPlot;
};

#endif