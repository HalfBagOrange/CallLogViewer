#ifndef CALLLOGVIEWER_H
#define CALLLOGVIEWER_H

#include <QMainWindow>
#include <QDomElement>
#include <QSqlTableModel>
#include "ui_CallLogViewer.h"

class QMySqlTableModel;

class CallLogViewer : public QMainWindow
{
	Q_OBJECT

public:
	CallLogViewer(QWidget *parent = 0, Qt::WindowFlags flags = 0);
	~CallLogViewer();

public slots:
	void slotChart();
	void slotClearData();
	void slotImportFromWeb();
	void slotExportToFile();
	void slotUpdateSqlModel();

private:
	Ui::CallLogViewerClass ui;
	QMySqlTableModel* m_sqlTableModel;
};

#endif // CALLLOGVIEWER_H
