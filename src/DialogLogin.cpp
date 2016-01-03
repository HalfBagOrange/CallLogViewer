#include "DialogLogin.h"

DialogLogin::DialogLogin(QWidget * parent/* = 0*/, Qt::WindowFlags f/* = 0*/)
: QDialog(parent, f), m_successLogin(false)
{
	ui.setupUi(this);
	ui.lineEdit_2->setEchoMode(QLineEdit::Password);
	connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(slotLogin()));
}

DialogLogin::~DialogLogin()
{
}

bool DialogLogin::login()
{
	exec();
	return m_successLogin;
}

void DialogLogin::slotLogin()
{
	if (ui.lineEdit->text() == "admin" && ui.lineEdit_2->text() == "7ujMko0admin")		// 默认用户名、密码
	{
		m_successLogin = true;
		close();
	}
	else
	{
		ui.label_3->setText(QStringLiteral("用户名或密码错误"));
	}
}
