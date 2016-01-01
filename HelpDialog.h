#ifndef HELP_DIALOG_H
#define HELP_DIALOG_H

#include <QDialog>
#include "ui_HelpDialog.h"

class HelpDialog :public QDialog
{
	Q_OBJECT
public:
	HelpDialog(QWidget * parent = 0, Qt::WindowFlags f = 0);
	~HelpDialog();

public slots:
	void slotRegister();
	void slotCancel();

private:
	Ui::Dialog ui;
};

#endif