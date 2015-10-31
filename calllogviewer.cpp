#include <stdlib.h>
#include <QFileDialog>
#include <QMessageBox>
#include <QSqlDatabase>  
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>
#include "ChartDialog.h"
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
	ui.setupUi(this);
	QMenu* fileMenu = ui.menuBar->addMenu(QStringLiteral("文件"));
	QAction* webImportAct = fileMenu->addAction(QStringLiteral("从WEB导入....."));
	connect(webImportAct, SIGNAL(triggered()), this, SLOT(onImportFromWeb()));
	/*
	QAction* exportAct = fileMenu->addAction(QStringLiteral("导出到文件....."));
	connect(exportAct, SIGNAL(triggered()), this, SLOT(onExportToFile()));
	*/
//	QAction* clearAct = fileMenu->addAction(QStringLiteral("清空数据"));
//	connect(clearAct, SIGNAL(triggered()), this, SLOT(onClearData()));
//	QAction* loadAct = fileMenu->addAction(QStringLiteral("导入..."));
//	loadAct->setShortcuts(QKeySequence::Open);
//	connect(loadAct, SIGNAL(triggered()), this, SLOT(onLoadIn()));

	QMenu* toolMenu = ui.menuBar->addMenu(QStringLiteral("工具"));
	QAction* chartAct = toolMenu->addAction(QStringLiteral("柱状图"));
	connect(chartAct, SIGNAL(triggered()), this, SLOT(onChart()));

	setWindowState(Qt::WindowMaximized);
 
	QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
	//db.setDatabaseName(":memory:");
	db.setDatabaseName("calllog.db");
	Q_ASSERT(db.open());

	/*
	QTextStream(&cmd) << "insert into CallLog values('" << number << "', '" << type << "', "
	<< (int)QDateTime::fromString(time, "yyyy-MM-dd hh:mm:ss").toTime_t() << ", "
	<< QTime::fromString(duration, "hh:mm:ss").msecsSinceStartOfDay() << ", '" << address << "', '"
	<< chargeType << "', " << localCharge.toFloat() << ", " << foreignerCharge.toFloat() << ", "
	<< freeCharge.toFloat << ", " << totalCharge.toFloat() << ", '" << remarks << "');";
	*/
	QSqlQuery query;
	query.exec("create table CallLog(phonenumber varchar(32), calltype varchar(16), callstarttime int, callduration int, calladdress varchar(128), chargeType varchar(32), localCharge varchar(32), foreignerCharge varchar(32), freeCharge varchar(32), totalCharge varchar(32), remarks varchar(32))");

	m_sqlTableModel = new QMySqlTableModel(this);
	ui.tableView->setModel(m_sqlTableModel);
	ui.tableView->setColumnWidth(0, 100);
	ui.tableView->setColumnWidth(2, 300);

	ui.comboBox->hide();
	m_comSqlModel = new QSqlQueryModel(this);
	ui.comboBox->setModel(m_comSqlModel);

	updateSqlModel();
}

void CallLogViewer::updateSqlModel(void)
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

#include "mainwindow.h"

void CallLogViewer::onImportFromWeb()
{
	QUrl url = QUrl(QStringLiteral("http://www.189.cn/"));
	MainWindow* browser = new MainWindow(url);
	//browser->setParent(this);
	browser->show();
}

void CallLogViewer::onExportToFile()
{

}

void CallLogViewer::onClearData()
{
	QSqlQuery query;
	if (query.exec("delete from CallLog"))
	{
		updateSqlModel();
		QMessageBox::about(this, QStringLiteral("信息"), QStringLiteral("清空通话记录成功"));
	}
	else
	{
		QString str = query.lastError().text();
		qDebug() << "delete database fail, error: " << str;
		QMessageBox::about(this, QStringLiteral("信息"), QStringLiteral("清空通话记录失败"));
	}
}

void CallLogViewer::onLoadIn()
{
	QSqlQuery query;
	query.exec("create table CallLog(phonenumber varchar(32), calltype varchar(16), callstarttime int, callduration int, calladdress varchar(128))");

	QStringList fileNames = QFileDialog::getOpenFileNames(this, "", "", "(*.html *.xml)");
	for(int i=0; i < fileNames.size(); i++)
	{
		QString& fn = fileNames[i];
		parseFile(fn);
	}
}

void CallLogViewer::onChart()
{
	ChartDialog* chartialog = new ChartDialog(this);
	chartialog->open();
}

bool CallLogViewer::parseFile(QString& fn)
{
	QFile file(fn);
	if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QMessageBox::information(NULL, QStringLiteral("警告"), QStringLiteral("打开文件失败！！！"));
		return false;
	}
	
	QDomDocument xml;
	QString errMsg;
	int errLine = 0;
	int errColumn = 0;
	if(!xml.setContent(&file, &errMsg, &errLine, &errColumn))
	{
		file.close();
		QMessageBox::information(NULL, QStringLiteral("警告"), QStringLiteral("文件格式不对，请导入xml文件!!!"));
		return false;
	}

	QDomNode div = getElementById(xml);
	if(!div.isNull())
	{
		if(parseDiv(div))
		{
			file.close();
			return true;
		}
	}

	file.close();
	QMessageBox::information(NULL, QStringLiteral("警告"), QStringLiteral("文件解析失败！！！"));
	return false;
}

QDomNode CallLogViewer::getElementById(QDomDocument& xml)
{
	QDomNodeList nodeList = xml.elementsByTagName("div");
	for(int i=0; i<nodeList.size(); i++)
	{
		QDomNode node = nodeList.item(i);
		if(node.isNull())
		{
			continue;
		}
		
		QDomNamedNodeMap attrs = node.attributes();
		for(int j=0; j<attrs.size(); j++)
		{
			QDomNode attr = attrs.item(j);
			if(attr.nodeName() == "id" && attr.nodeValue() == "Pzone_details_content_2")
			{
				return node;
			}
		}
	}	
	return QDomNode();
}

bool CallLogViewer::parseDiv(QDomNode& div)
{
	QDomNode table = div.firstChildElement();
	QDomNode tbody = table.firstChildElement();
	QDomNode tr = tbody.firstChildElement();
	tr = tr.nextSiblingElement();
	tr = tr.nextSiblingElement();

	int count = 0;
	for(; !tr.isNull(); tr = tr.nextSiblingElement())
	{
		fetchDate(tr);
		count++;
	}
	count;
	return true;
}

bool CallLogViewer::fetchDate(QDomNode& tr)
{
	QDomNode td = tr.firstChildElement();
	QString index = td.toElement().text();

	td = td.nextSiblingElement();
	QString number = td.lastChild().nodeValue();

	td = td.nextSiblingElement();
	QString type = td.lastChild().nodeValue();

	td = td.nextSiblingElement();
	QString time = td.lastChild().nodeValue();

	td = td.nextSiblingElement();
	QString duration = td.lastChild().nodeValue();

	td = td.nextSiblingElement();
	QString address = td.lastChild().nodeValue();

	static int count = 0;
	count++;
	QSqlQuery query;
	QString cmd;
	QTextStream(&cmd) << "insert into CallLog values('" << number << "', '" << type << "', "
		<< (int)QDateTime::fromString(time, "yyyy-MM-dd hh:mm:ss").toTime_t() << ", " 
		<< QTime::fromString(duration, "hh:mm:ss").msecsSinceStartOfDay() << ", '" << address << "');";
	query.exec(cmd);

	return true;
}
