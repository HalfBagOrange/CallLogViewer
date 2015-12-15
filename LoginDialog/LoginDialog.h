#ifndef LOGIN_DIALOG_H
#define LOGIN_DIALOG_H

#include <QDialog>
#include "ui_LoginDialog.h"

class LoginDialog : public QDialog
{
	Q_OBJECT
public:
	LoginDialog(QWidget * parent = 0, Qt::WindowFlags f = 0);
	~LoginDialog();

	bool login();

public slots:
	void slotLogin();

private:
	Ui::Dialog ui;
	bool successLogin;
};

#endif