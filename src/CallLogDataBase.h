#ifndef CALL_LOG_DATA_BASE_H
#define CALL_LOG_DATA_BASE_H

#include <QSqlTableModel>
#include <QDateTime>

class CallLogTableModel : public QSqlTableModel
{
public:
	CallLogTableModel(QObject * parent = 0, QSqlDatabase db = QSqlDatabase())
		:QSqlTableModel(parent, db){}
	~CallLogTableModel(){}

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

#endif