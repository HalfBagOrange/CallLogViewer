#include "CallLogBrowser.h"

CallLogBrowser::CallLogBrowser(QWidget * parent/* = 0*/, Qt::WindowFlags flags/* = 0*/)
	:QMainWindow(parent, flags)
{
	ui.setupUi(this);
}

CallLogBrowser::~CallLogBrowser()
{
}