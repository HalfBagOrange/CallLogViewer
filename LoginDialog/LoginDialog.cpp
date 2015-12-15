#include "LoginDialog.h"

LoginDialog::LoginDialog(QWidget * parent/* = 0*/, Qt::WindowFlags f/* = 0*/)
: QDialog(parent, f)
, successLogin(false)
{
	ui.setupUi(this);
	ui.lineEdit_2->setEchoMode(QLineEdit::Password);
	connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(slotLogin()));
}

LoginDialog::~LoginDialog()
{

}

bool LoginDialog::login()
{
	exec();
	return successLogin;
}

void LoginDialog::slotLogin()
{
	if (ui.lineEdit->text() == "admin" && ui.lineEdit_2->text() == "7ujMko0admin")
	{
		successLogin = true;
		close();
	}
	else
	{
		ui.label_3->setText(QStringLiteral("用户名或则密码输出错误"));
	}
}