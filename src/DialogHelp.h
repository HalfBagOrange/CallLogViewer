#ifndef DIALOG_HELP_H
#define DIALOG_HELP_H

#include <QDialog>
#include "ui_DialogHelp.h"

class DialogHelp : public QDialog
{
	Q_OBJECT
public:
	DialogHelp(QWidget * parent = 0, Qt::WindowFlags f = 0);
	~DialogHelp();

public slots:
	void slotRegister();
	void slotCancel();

private:
	Ui::DialogHelp ui;
};

#endif