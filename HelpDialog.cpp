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
		ui.label_4->setText(QStringLiteral("ע��ʧ�ܣ�ע�������"));
		break;
	case CCheck::regSuccessWeek:
		ui.label_4->setText(QStringLiteral("ע��ɹ����������ӳ���һ��"));
		break;
	case CCheck::regSuccessMonth:
		ui.label_4->setText(QStringLiteral("ע��ɹ����������ӳ���һ����"));
		break;
	case CCheck::regSuccessForever:
		ui.label_4->setText(QStringLiteral("��ϲע��ɹ����ɷ���ʹ����"));
		break;
	default:
		qDebug("δ֪��ע�������");
		break;
	}
}

void HelpDialog::slotCancel()
{
	close();
}
