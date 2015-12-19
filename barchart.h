#ifndef _BAR_CHART_H_
#define _BAR_CHART_H_

#include <qwt_plot.h>
#include <qwt_plot_layout.h>
#include <qstringlist.h>
#include <QDialog>
#include <QSqlTableModel>
#include <QDateTime>

class DistroChartItem;
class DistroScaleDraw;

class QMySqlTableModel : public QSqlTableModel
{
public:
	QMySqlTableModel(QObject * parent = 0, QSqlDatabase db = QSqlDatabase())
		:QSqlTableModel(parent, db)
	{
	}
	~QMySqlTableModel()
	{
	}

	void mySetPrimaryKey(const QSqlIndex & key)
	{
		setPrimaryKey(key);
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

class MyPlotLayout : public QwtPlotLayout
{
public:
	MyPlotLayout() :QwtPlotLayout(){}
	~MyPlotLayout(){}

public:
	void mySetScaleRect(int axis, const QRectF &rect)
	{
		return setScaleRect(axis, rect);
	}
};

class BarChart: public QObject
{
    Q_OBJECT

public:
	BarChart(QObject* parent = NULL);
	void setPlot(QwtPlot* plot);

public Q_SLOTS:
	void setOrientation(int o);
	void addBar(const QString &distro, const QColor &color, double const& data);
	void clear();
	void replot();
	void exportChart(const QString &filename);

private:
	QwtPlot* m_plot;
    DistroChartItem* m_barChartItem;
	DistroScaleDraw* m_barScaleX;
	int m_count;
};

#endif
