#ifndef REGISTER_KEY_TOOL_H
#define REGISTER_KEY_TOOL_H

#include <QtWidgets/QMainWindow>
#include "ui_RegisterKeyTool.h"

class RegisterKeyTool : public QMainWindow
{
	Q_OBJECT

public:
	RegisterKeyTool(QWidget *parent = 0);
	~RegisterKeyTool();

public slots:
	void slotBuild();
	
private:
	Ui::RegisterKeyToolClass ui;
};

#endif // REGISTER_KEY_TOOL_H
