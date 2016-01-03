#include <QTextStream>
#include <QSqlDatabase>  
#include <QSqlQuery>

#include "WindowTable.h"

WindowTable::WindowTable(QWidget *parent, Qt::WindowFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);

	m_modelCallLog = new CallLogTableModel(this);
	ui.tableView->setModel(m_modelCallLog);

	m_modelPhoneNumber = new QSqlQueryModel(this);
	ui.comboBox->setModel(m_modelPhoneNumber);
	
	connect(ui.comboBox, SIGNAL(activated(const QString &)), this, SLOT(slotPhoneNumberChanged(const QString &)));

	slotCallLogChanged();
}

WindowTable::~WindowTable()
{
}

void WindowTable::slotPhoneNumberChanged(const QString & text)
{
	QString cmd;
	QTextStream(&cmd) << "callnumber = " << text;
	m_modelCallLog->setFilter(cmd);
}

void WindowTable::slotCallLogChanged(void)
{
	m_modelCallLog->setTable("CallLog");
	m_modelCallLog->setEditStrategy(QSqlTableModel::OnManualSubmit);
	m_modelCallLog->select();

	m_modelCallLog->setHeaderData(0, Qt::Horizontal, QVariant(QStringLiteral("对方号码")));
	m_modelCallLog->setHeaderData(1, Qt::Horizontal, QVariant(QStringLiteral("通话类型")));
	m_modelCallLog->setHeaderData(2, Qt::Horizontal, QVariant(QStringLiteral("起始时间")));
	m_modelCallLog->setHeaderData(3, Qt::Horizontal, QVariant(QStringLiteral("通话时长")));
	m_modelCallLog->setHeaderData(4, Qt::Horizontal, QVariant(QStringLiteral("通话地")));
	m_modelCallLog->setHeaderData(5, Qt::Horizontal, QVariant(QStringLiteral("对方通话所在地")));
	m_modelCallLog->setHeaderData(6, Qt::Horizontal, QVariant(QStringLiteral("通话所在LAC")));
	m_modelCallLog->setHeaderData(7, Qt::Horizontal, QVariant(QStringLiteral("通话所在CELLID")));

	QSqlQuery query;
	query.exec("SELECT DISTINCT callnumber FROM CallLog");
	m_modelPhoneNumber->setQuery(query);
}
