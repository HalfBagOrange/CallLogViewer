#ifndef WINDOW_TABLE_H
#define WINDOW_TABLE_H

#include <QMainWindow>
#include <QSqlQueryModel>
#include "CallLogDataBase.h"
#include "ui_WindowTable.h"

class WindowTable : public QMainWindow
{
	Q_OBJECT

public:
	WindowTable(QWidget *parent = 0, Qt::WindowFlags flags = 0);
	~WindowTable();

public slots:
	void slotCallLogChanged();
	void slotPhoneNumberChanged(const QString & text);

private:
	Ui::WindowTable ui;

	CallLogTableModel* m_modelCallLog;
	QSqlQueryModel* m_modelPhoneNumber;
};

#endif // WINDOW_TABLE_H
