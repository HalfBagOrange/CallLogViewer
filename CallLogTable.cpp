#include <stdlib.h>
#include <QFileDialog>
#include <QMessageBox>
#include <QSqlDatabase>  
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>
#include "ChartDialog.h"
#include "mainwindow.h"
#include "CallLogViewer.h"

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

CallLogViewer::CallLogViewer(QWidget *parent, Qt::WindowFlags flags)
	: QMainWindow(parent, flags)
{
//	setWindowState(Qt::WindowMaximized);

	ui.setupUi(this);

	/*
	QMenu* fileMenu = ui.menuBar->addMenu(QStringLiteral("文件"));
	QAction* webImportAct = fileMenu->addAction(QStringLiteral("从WEB导入....."));
	connect(webImportAct, SIGNAL(triggered()), this, SLOT(slotImportFromWeb()));
	*/

//	QAction* exportAct = fileMenu->addAction(QStringLiteral("导出到文件....."));
//	connect(exportAct, SIGNAL(triggered()), this, SLOT(slotExportToFile()));
//	QAction* clearAct = fileMenu->addAction(QStringLiteral("清空数据"));
//	connect(clearAct, SIGNAL(triggered()), this, SLOT(slotClearData()));
	/*
	QMenu* toolMenu = ui.menuBar->addMenu(QStringLiteral("工具"));
	QAction* chartAct = toolMenu->addAction(QStringLiteral("柱状图"));
	connect(chartAct, SIGNAL(triggered()), this, SLOT(slotChart()));
	*/

	/*
	QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
//	db.setDatabaseName(":memory:");
	db.setDatabaseName("calllog.db");

	QSqlQuery query;
	query.exec("create table CallLog(phonenumber varchar(32), calltype varchar(16), callstarttime int, callduration int, calladdress varchar(128), chargetype varchar(32), localcharge varchar(32), foreignercharge varchar(32), freecharge varchar(32), totalcharge varchar(32), remarks varchar(32))");
	*/
	m_sqlTableModel = new QMySqlTableModel(this);
	ui.tableView->setModel(m_sqlTableModel);
//	ui.tableView->setColumnWidth(0, 100);
//	ui.tableView->setColumnWidth(2, 300);

	m_comSqlModel = new QSqlQueryModel(this);
	ui.comboBox->setModel(m_comSqlModel);
	
	connect(ui.comboBox, SIGNAL(activated(const QString &)), this, SLOT(slotPhoneNumberChanged(const QString &)));

	slotUpdateSqlModel();
}

void CallLogViewer::slotPhoneNumberChanged(const QString & text)
{
	QString cmd;
	QTextStream(&cmd) << "phonenumber = " << text;
	m_sqlTableModel->setFilter(cmd);
}

void CallLogViewer::slotUpdateSqlModel(void)
{
	m_sqlTableModel->setTable("CallLog");
	m_sqlTableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
	m_sqlTableModel->select();

	m_sqlTableModel->setHeaderData(0, Qt::Horizontal, QVariant(QStringLiteral("对方号码")));
	m_sqlTableModel->setHeaderData(1, Qt::Horizontal, QVariant(QStringLiteral("呼叫类型")));
	m_sqlTableModel->setHeaderData(2, Qt::Horizontal, QVariant(QStringLiteral("通话日期起始时间")));
	m_sqlTableModel->setHeaderData(3, Qt::Horizontal, QVariant(QStringLiteral("通话时长")));
	m_sqlTableModel->setHeaderData(4, Qt::Horizontal, QVariant(QStringLiteral("通话地")));
	m_sqlTableModel->setHeaderData(5, Qt::Horizontal, QVariant(QStringLiteral("通话类型")));
	m_sqlTableModel->setHeaderData(6, Qt::Horizontal, QVariant(QStringLiteral("本地费或漫游费")));
	m_sqlTableModel->setHeaderData(7, Qt::Horizontal, QVariant(QStringLiteral("长途费")));
	m_sqlTableModel->setHeaderData(8, Qt::Horizontal, QVariant(QStringLiteral("减免")));
	m_sqlTableModel->setHeaderData(9, Qt::Horizontal, QVariant(QStringLiteral("费用小计")));
	m_sqlTableModel->setHeaderData(10, Qt::Horizontal, QVariant(QStringLiteral("备注")));

	QSqlQuery query;
	query.exec("SELECT DISTINCT phonenumber FROM CallLog");
	m_comSqlModel->setQuery(query);
}

CallLogViewer::~CallLogViewer()
{
}

void CallLogViewer::slotImportFromWeb()
{
	QUrl url = QUrl(QStringLiteral("http://www.189.cn/"));
	MainWindow* browser = new MainWindow(url);

	connect(browser, SIGNAL(sqlDatachanged()), this, SLOT(updateSqlModel()));
	browser->show();
}

void CallLogViewer::slotExportToFile()
{
}

void CallLogViewer::slotClearData()
{
	QSqlQuery query;
	if (query.exec("delete from CallLog"))
	{
		slotUpdateSqlModel();
		QMessageBox::about(this, QStringLiteral("信息"), QStringLiteral("清空通话记录成功"));
	}
	else
	{
		QString str = query.lastError().text();
		qDebug() << "delete database fail, error: " << str;
		QMessageBox::about(this, QStringLiteral("信息"), QStringLiteral("清空通话记录失败"));
	}
}

void CallLogViewer::slotChart()
{
	ChartDialog* chartialog = new ChartDialog();
	chartialog->show();
}
