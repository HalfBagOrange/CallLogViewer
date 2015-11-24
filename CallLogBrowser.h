#ifndef CALL_LOG_BROWSER_H
#define CALL_LOG_BROWSER_H

#include <QMainWindow>
#include "ui_CallLogBrowser.h"

class CallLogBrowser : public QMainWindow
{
    Q_OBJECT

public:
	CallLogBrowser(QWidget * parent = 0, Qt::WindowFlags flags = 0);
	~CallLogBrowser();

private:
	Ui::CallLogBrowser ui;
};

#endif