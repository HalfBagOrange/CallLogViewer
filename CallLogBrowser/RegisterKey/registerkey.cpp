#include "registerkey.h"
#include <QCryptographicHash>

RegisterKey::RegisterKey(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	ui.comboBox->addItem(QStringLiteral("一周"));
	ui.comboBox->addItem(QStringLiteral("一个月"));
	ui.comboBox->addItem(QStringLiteral("永久"));
	connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(slotBuild()));
}

RegisterKey::~RegisterKey()
{
}

void RegisterKey::slotBuild()
{
	QString seriseKey = ui.lineEdit->text();

	switch (ui.comboBox->currentIndex())
	{
	case 0:
		ui.lineEdit_2->setText(QCryptographicHash::hash(seriseKey.toStdString().c_str(), QCryptographicHash::Md5).toHex());
		break;
	case 1:
		ui.lineEdit_2->setText(QCryptographicHash::hash(seriseKey.toStdString().c_str(), QCryptographicHash::Sha1).toHex());
		break;
	case 2:
		ui.lineEdit_2->setText(QCryptographicHash::hash(seriseKey.toStdString().c_str(), QCryptographicHash::Sha224).toHex());
		break;
	default:
		qWarning("unknow type\n");
		break;
	}
}