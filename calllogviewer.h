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
	void onLoadIn();
	void onChart();
	void onClearData();
	void onImportFromWeb();
	void onExportToFile();

private:
	QDomNode getElementById(QDomDocument& xml);
	bool parseFile(QString& fn);
	bool parseDiv(QDomNode& div);
	bool fetchDate(QDomNode& tr);
	void updateSqlModel(void);

public:
	QMySqlTableModel* m_sqlTableModel;
	QSqlQueryModel* m_comSqlModel;

private:
	Ui::CallLogViewerClass ui;
	
};

#endif // CALLLOGVIEWER_H
