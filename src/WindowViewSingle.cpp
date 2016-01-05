#include <QSqlQuery>
#include "WindowViewSingle.h"


WindowViewSingle::WindowViewSingle(QWidget *parent, Qt::WindowFlags flags)
: QMainWindow(parent, flags)
{
	ui.setupUi(this);

	ui.dateEdit_3->setCalendarPopup(true);
	ui.dateEdit_4->setCalendarPopup(true);

	m_modelPhoneNumber = new QSqlQueryModel(this);
	ui.comboBox->setModel(m_modelPhoneNumber);

	connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(slotDateTimeChanged()));
	connect(ui.comboBox, SIGNAL(activated(const QString &)), this, SLOT(slotPhoneNumberChanged(const QString &)));

	slotCallLogChanged();
}

WindowViewSingle::~WindowViewSingle()
{
}

void WindowViewSingle::slotCallLogChanged()
{
	QSqlQuery query;
	query.exec("SELECT DISTINCT callnumber FROM CallLog");
	m_modelPhoneNumber->setQuery(query);

	slotPhoneNumberChanged(ui.comboBox->currentData().toString());
}

void WindowViewSingle::slotPhoneNumberChanged(const QString & text)
{
	QString cmd;
	QSqlQuery query;

	QTextStream(&cmd) << "SELECT callnumber = " << text << "And MIN(starttime) AS starttime FROM CallLog";
	query.exec(cmd);
	query.first();
	QVariant minDate = query.value("starttime");
	if (minDate.toUInt() == 0)
	{
		minDate = QDateTime::currentDateTime().toTime_t();
	}

	QTextStream(&cmd) << "SELECT callnumber = " << text << "And MAX(starttime) AS starttime FROM CallLog";
	query.exec(cmd);
	query.first();
	QVariant maxDate = query.value("starttime");
	if (maxDate.toUInt() == 0)
	{
		maxDate = QDateTime::currentDateTime().toTime_t();
	}

	ui.dateEdit_3->setDate(QDateTime::fromTime_t(minDate.toUInt()).date());
	//	ui.dateEdit->setDateRange(QDateTime::fromTime_t(minDate.toUInt()).date(), QDateTime::fromTime_t(maxDate.toUInt()).date());

	ui.dateEdit_4->setDate(QDateTime::fromTime_t(maxDate.toUInt()).date());
	//	ui.dateEdit_2->setDateRange(QDateTime::fromTime_t(minDate.toUInt()).date(), QDateTime::fromTime_t(maxDate.toUInt()).date());

	slotDateTimeChanged();
}

void WindowViewSingle::slotDateTimeChanged()
{
}