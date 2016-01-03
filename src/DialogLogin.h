#ifndef DIALOG_LOGIN_H
#define DIALOG_LOGIN_H

#include <QDialog>
#include "ui_DialogLogin.h"

class DialogLogin : public QDialog
{
	Q_OBJECT
public:
	DialogLogin(QWidget * parent = 0, Qt::WindowFlags f = 0);
	~DialogLogin();
	bool login();

public slots:
	void slotLogin();

private:
	Ui::DialogLogin ui;
	bool m_successLogin;
};

#endif