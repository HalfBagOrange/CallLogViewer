#ifndef WINDOW_VIEW_SINGLE_H
#define WINDOW_VIEW_SINGLE_H

#include <QMainWindow>
#include <QSqlQueryModel>
#include "ui_WindowViewSingle.h"

class WindowViewSingle :public QMainWindow
{
	Q_OBJECT
public:
	WindowViewSingle(QWidget *parent = 0, Qt::WindowFlags flags = 0);
	~WindowViewSingle();

public slots:
	void slotCallLogChanged();
	void slotPhoneNumberChanged(const QString & text);
	void slotDateTimeChanged();

private:
	Ui::WindowViewSingle ui;
	QSqlQueryModel* m_modelPhoneNumber;
};

#endif