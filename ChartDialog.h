#ifndef CHARTDIALOG_H
#define CHARTDIALOG_H

#include <QDialog>
#include "ui_ChartDialog.h"
#include "barchart.h"
#include <QSqlDatabase>  
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>
#include <QSqlTableModel>

class ChartDialog : public QDialog
{
	Q_OBJECT
public:
	ChartDialog(QWidget *parent = 0, Qt::WindowFlags flags = 0);
	~ChartDialog();

public slots:
	void statsButtonClicked();

private:
	BarChart* m_barChart;
	Ui::ChartDialog ui;
	QSqlQueryModel* m_sqlTableModel;
};

#endif