#ifndef CHARTDIALOG_H
#define CHARTDIALOG_H

#include <QMainWindow>
#include <QDialog>
#include "ui_ChartDialog.h"
#include "barchart.h"
#include <QSqlDatabase>  
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>
#include <QSqlTableModel>

class ChartDialog : public QMainWindow
{
	Q_OBJECT
public:
	ChartDialog(QWidget *parent = 0, Qt::WindowFlags flags = 0);
	~ChartDialog();

public slots:
//	void statsButtonClicked();
	void slotDateChanged();
	void slotImportFromWeb();
	void slotDateList();
	void slotCallLogChanged();
	void slotPhoneNumberChanged(const QString & text);
	void slotAbort();
	void slotTrialTimeout();
	void slotSuccessRegisted();

private:
	BarChart* m_timeChart;
	BarChart* m_countChart;
	Ui::ChartDialog ui;

	QMySqlTableModel* m_sqlTableModel;
	QSqlQueryModel* m_comSqlModel;

	bool m_trialTimeout;
};

#endif