#ifndef BROWSER_MAIN_WINDOW_H
#define BROWSER_MAIN_WINDOW_H

#include <QtWidgets>

class BrowserMainWindow : public QMainWindow
{
    Q_OBJECT

public:
	BrowserMainWindow(QWidget *parent = 0, Qt::WindowFlags flags = 0);
	~BrowserMainWindow();

protected slots:
			
};

#endif