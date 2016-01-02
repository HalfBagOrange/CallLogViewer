#ifndef REGISTERKEY_H
#define REGISTERKEY_H

#include <QtWidgets/QMainWindow>
#include "ui_registerkey.h"

class RegisterKey : public QMainWindow
{
	Q_OBJECT

public:
	RegisterKey(QWidget *parent = 0);
	~RegisterKey();

public slots:
	void slotBuild();
	
private:
	Ui::RegisterKeyClass ui;
};

#endif // REGISTERKEY_H
