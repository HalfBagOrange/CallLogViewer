#include <stdlib.h>
#include <QDebug>
#include <QMessageBox>
#include <QDateTime>
#include <QSqlQuery>
#include "CallLogTable.h"

class CallLogTableModel : public QSqlTableModel
{
public:
	CallLogTableModel(QObject * parent = 0, QSqlDatabase db = QSqlDatabase())
		:QSqlTableModel(parent, db)
	{
	}
	~CallLogTableModel()
	{
	}
protected:
	QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const
	{
		QVariant value = QSqlQueryModel::data(index, role);

		if (value.isValid() && Qt::DisplayRole == role)
		{
			if (2 == index.column())
			{
				return QVariant(QDateTime::fromTime_t(value.toUInt()).toString("yyyy-MM-dd hh:mm:ss"));
			}
			else if (3 == index.column())
			{
				return QVariant(QTime::fromMSecsSinceStartOfDay(value.toUInt()).toString("hh:mm:ss"));
			}
		}
		return value;
	}
};

CallLogTable::CallLogTable(QWidget *parent, Qt::WindowFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);

	m_callLogModel = new CallLogTableModel(this);
	ui.tableView->setModel(m_callLogModel);

	m_phoneNumberModel = new QSqlQueryModel(this);
	ui.comboBox->setModel(m_phoneNumberModel);
	
	connect(ui.comboBox, SIGNAL(activated(const QString &)), this, SLOT(slotPhoneNumberChanged(const QString &)));

	slotCallLogChanged();
}

void CallLogTable::slotPhoneNumberChanged(const QString & text)
{
	QString cmd;
	QTextStream(&cmd) << "phonenumber = " << text;
	m_callLogModel->setFilter(cmd);
}

void CallLogTable::slotCallLogChanged()
{
	m_callLogModel->setTable("CallLog");
	m_callLogModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
	m_callLogModel->select();

	m_callLogModel->setHeaderData(0, Qt::Horizontal, QVariant(QStringLiteral("对方号码")));
	m_callLogModel->setHeaderData(1, Qt::Horizontal, QVariant(QStringLiteral("呼叫类型")));
	m_callLogModel->setHeaderData(2, Qt::Horizontal, QVariant(QStringLiteral("通话日期起始时间")));
	m_callLogModel->setHeaderData(3, Qt::Horizontal, QVariant(QStringLiteral("通话时长")));
	m_callLogModel->setHeaderData(4, Qt::Horizontal, QVariant(QStringLiteral("通话地")));
	m_callLogModel->setHeaderData(5, Qt::Horizontal, QVariant(QStringLiteral("通话类型")));
	m_callLogModel->setHeaderData(6, Qt::Horizontal, QVariant(QStringLiteral("本地费或漫游费")));
	m_callLogModel->setHeaderData(7, Qt::Horizontal, QVariant(QStringLiteral("长途费")));
	m_callLogModel->setHeaderData(8, Qt::Horizontal, QVariant(QStringLiteral("减免")));
	m_callLogModel->setHeaderData(9, Qt::Horizontal, QVariant(QStringLiteral("费用小计")));
	m_callLogModel->setHeaderData(10, Qt::Horizontal, QVariant(QStringLiteral("备注")));

	QSqlQuery query;
	query.exec("SELECT DISTINCT phonenumber FROM CallLog");
	m_phoneNumberModel->setQuery(query);
}

CallLogTable::~CallLogTable()
{
}
