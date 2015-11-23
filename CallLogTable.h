#ifndef CALL_LOG_TABLE_H
#define CALL_LOG_TABLE_H

#include <QMainWindow>
#include <QSqlTableModel>
#include <QSqlQueryModel>

#include "ui_CallLogTable.h"

class CallLogTableModel;

class CallLogTable : public QMainWindow
{
	Q_OBJECT

public:
	CallLogTable(QWidget *parent = 0, Qt::WindowFlags flags = 0);
	~CallLogTable();

public slots:
	void slotCallLogChanged();
	void slotPhoneNumberChanged(const QString & text);

private:
	Ui::CallLogTable ui;

	CallLogTableModel* m_callLogModel;
	QSqlQueryModel* m_phoneNumberModel;
};

#endif // CALLLOGVIEWER_H
