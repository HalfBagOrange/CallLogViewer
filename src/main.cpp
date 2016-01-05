#include <QFile>
#include <QTextCodec>
#include <QSqlDatabase>  
#include <QSqlQuery>

#include "ObjectRegister.h"
#include "WindowMain.h"

static void MessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
	QByteArray localMsg = msg.toLocal8Bit();
	QFile logFile("log.txt");
	logFile.open(QIODevice::Append);
	if (logFile.size() > 5 * 1024 * 1024)
	{
		logFile.seek(0);
	}

	QString text;
	switch (type) 
	{
	case QtDebugMsg:
		QTextStream(&text) << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << " " << "Debug: " << localMsg.constData() << " (" << context.file << ":" << context.line << ", " << context.function << ")\n";
		break;
	case QtInfoMsg:
		QTextStream(&text) << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << " " << "Info: " << localMsg.constData() << " (" << context.file << ":" << context.line << ", " << context.function << ")\n";
		break;
	case QtWarningMsg:
		QTextStream(&text) << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << " " << "Warning: " << localMsg.constData() << " (" << context.file << ":" << context.line << ", " << context.function << ")\n";
		break;
	case QtCriticalMsg:
		QTextStream(&text) << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << " " << "Critical: " << localMsg.constData() << " (" << context.file << ":" << context.line << ", " << context.function << ")\n";
		break;
	case QtFatalMsg:
		QTextStream(&text) << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << " " << "Fatal: " << localMsg.constData() << " (" << context.file << ":" << context.line << ", " << context.function << ")\n";
		break;
	default:
		qFatal("unknow error type: %d\n", (int)type);
		return;
	}
	logFile.write(text.toUtf8().constData());
	logFile.close();
}

int main(int argc, char * argv[])
{
	qInstallMessageHandler(MessageOutput);

    QApplication app(argc, argv);
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));

	QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
	//db.setDatabaseName(":memory:");
	db.setDatabaseName("calllog.db");
	db.open();

	QSqlQuery query;
	query.exec("create table CallLog(callnumber varchar(32), calltype varchar(16), starttime int, duration int, calllocation varchar(128), otherCallLocation varchar(32), callInLac varchar(32), callInCellid varchar(32))");

	WindowMain viewer;
	viewer.show();

//	ObjectRegister::instance()->init();

    return app.exec();
}
