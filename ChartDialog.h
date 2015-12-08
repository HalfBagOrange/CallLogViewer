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

private:
	BarChart* m_timeChart;
	BarChart* m_countChart;
	Ui::ChartDialog ui;
	//QSqlQueryModel* m_sqlTableModel;
};

#endif