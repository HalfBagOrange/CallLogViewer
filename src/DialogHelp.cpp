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
		ui.label_4->setText(QStringLiteral("ע��ʧ�ܣ�ע�������"));
		break;
	case ObjectRegister::registerSuccessWeek:
		ui.label_4->setText(QStringLiteral("ע��ɹ����������ӳ���һ��"));
		break;
	case ObjectRegister::registerSuccessMonth:
		ui.label_4->setText(QStringLiteral("ע��ɹ����������ӳ���һ����"));
		break;
	case ObjectRegister::registerSuccessForever:
		ui.label_4->setText(QStringLiteral("��ϲ��ע��ɹ����ɷ���ʹ����"));
		break;
	default:
		qDebug("δ֪��ע�������");
		break;
	}
}

void DialogHelp::slotCancel()
{
	close();
}
