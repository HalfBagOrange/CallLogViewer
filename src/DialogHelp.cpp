#include "ObjectRegister.h"
#include "DialogHelp.h"

DialogHelp::DialogHelp(QWidget * parent/* = 0*/, Qt::WindowFlags f/* = 0*/)
:QDialog(parent, f)
{
	ui.setupUi(this);
	ui.lineEdit_2->setText(ObjectRegister::instance()->getSoftSeriseKey());

	connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(slotRegister()));
	connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(slotCancel()));
}

DialogHelp::~DialogHelp()
{
}

void DialogHelp::slotRegister()
{
	switch (ObjectRegister::instance()->softRegister(ui.lineEdit->text()))
	{
	case ObjectRegister::registerFail:
		ui.label_4->setText(QStringLiteral("注册失败，注册码错误"));
		break;
	case ObjectRegister::registerSuccessWeek:
		ui.label_4->setText(QStringLiteral("注册成功，试用期延长至一周"));
		break;
	case ObjectRegister::registerSuccessMonth:
		ui.label_4->setText(QStringLiteral("注册成功，试用期延长至一个月"));
		break;
	case ObjectRegister::registerSuccessForever:
		ui.label_4->setText(QStringLiteral("恭喜您注册成功，可放心使用了"));
		break;
	default:
		qDebug("未知的注册错误码");
		break;
	}
}

void DialogHelp::slotCancel()
{
	close();
}
