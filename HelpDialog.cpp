#include "Check.h"
#include "HelpDialog.h"

HelpDialog::HelpDialog(QWidget * parent/* = 0*/, Qt::WindowFlags f/* = 0*/)
{
	ui.setupUi(this);
	ui.lineEdit_2->setText(CCheck::instance()->getSoftSeriseKey());

	connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(slotRegister()));
	connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(slotCancel()));
}

HelpDialog::~HelpDialog()
{
}

void HelpDialog::slotRegister()
{
	switch (CCheck::instance()->softRegister(ui.lineEdit->text()))
	{
	case CCheck::regFail:
		ui.label_4->setText(QStringLiteral("注册失败，注册码错误"));
		break;
	case CCheck::regSuccessWeek:
		ui.label_4->setText(QStringLiteral("注册成功，试用期延长至一周"));
		break;
	case CCheck::regSuccessMonth:
		ui.label_4->setText(QStringLiteral("注册成功，试用期延长至一个月"));
		break;
	case CCheck::regSuccessForever:
		ui.label_4->setText(QStringLiteral("恭喜注册成功，可放心使用了"));
		break;
	default:
		qDebug("未知的注册错误码");
		break;
	}
}

void HelpDialog::slotCancel()
{
	close();
}
