#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QFile>
#include "CallLogChart.h"

static void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
	QByteArray localMsg = msg.toLocal8Bit();
	QFile logFile("log.txt");
	logFile.open(QIODevice::Append);
	if (logFile.size() > 5 * 1024 * 1024)
	{
		logFile.seek(0);
	}
	QString text;
	switch (type) {
	case QtDebugMsg:
		QTextStream(&text) << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << " " << "Debug: " << localMsg.constData() << " (" << context.file << ":" << context.line << ", " << context.function << ")\n";
		//fprintf(stderr, "Debug: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
		break;
	case QtInfoMsg:
		QTextStream(&text) << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << " " << "Info: " << localMsg.constData() << " (" << context.file << ":" << context.line << ", " << context.function << ")\n";
		//fprintf(stderr, "Info: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
		break;
	case QtWarningMsg:
		QTextStream(&text) << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << " " << "Warning: " << localMsg.constData() << " (" << context.file << ":" << context.line << ", " << context.function << ")\n";
		//fprintf(stderr, "Warning: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
		break;
	case QtCriticalMsg:
		QTextStream(&text) << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << " " << "Critical: " << localMsg.constData() << " (" << context.file << ":" << context.line << ", " << context.function << ")\n";
		//fprintf(stderr, "Critical: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
		break;
	case QtFatalMsg:
		QTextStream(&text) << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << " " << "Fatal: " << localMsg.constData() << " (" << context.file << ":" << context.line << ", " << context.function << ")\n";
		//fprintf(stderr, "Fatal: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
		//abort();
	}
	fprintf(stderr, "%s", text.toUtf8().constData());
	logFile.write(text.toUtf8().constData());
	logFile.close();
}

int main(int argc, char * argv[])
{
	qInstallMessageHandler(myMessageOutput);
    QApplication app(argc, argv);

	QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
	//db.setDatabaseName(":memory:");
	db.setDatabaseName("calllog.db");
	db.open();
	QSqlQuery query;
	query.exec("create table CallLog(phonenumber varchar(32), calltype varchar(16), callstarttime int, callduration int, calladdress varchar(128), chargetype varchar(32), localcharge varchar(32), foreignercharge varchar(32), freecharge varchar(32), totalcharge varchar(32), remarks varchar(32))");

	CallLogChart chart;
	chart.show();
    return app.exec();
}
