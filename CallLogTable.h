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
	void slotPhoneNumberChanged(const QString & text);

private:
	Ui::CallLogViewerClass ui;
	QMySqlTableModel* m_sqlTableModel;
	QSqlQueryModel* m_comSqlModel;
};

#endif // CALLLOGVIEWER_H
