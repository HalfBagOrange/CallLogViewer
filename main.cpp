/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtWidgets>
#include "mainwindow.h"
#include "ChartDialog.h"
#include <QSqlDatabase>  
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>
#include "LoginDialog/LoginDialog.h"

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

static void showHelp(QCommandLineParser &parser, const QString errorMessage = QString())
{
    QString text;
    QTextStream str(&text);
    str << "<html><head/><body>";
    if (!errorMessage.isEmpty())
        str << "<p>" << errorMessage << "</p>";
    str << "<pre>" << parser.helpText() << "</pre></body></html>";
    QMessageBox box(errorMessage.isEmpty() ? QMessageBox::Information : QMessageBox::Warning,
        QGuiApplication::applicationDisplayName(), text, QMessageBox::Ok);
    box.setTextInteractionFlags(Qt::TextBrowserInteraction);
    box.exec();
}

int main(int argc, char * argv[])
{
	qInstallMessageHandler(myMessageOutput);
    QApplication app(argc, argv);

	QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));

#if 0
	LoginDialog loginDialog;

	if (!loginDialog.login())
	{
		return 0;
	}
#endif
	/*
    QCommandLineParser commandLineParser;
    commandLineParser.addPositionalArgument(QStringLiteral("url"),
        QStringLiteral("The url to be loaded in the browser window."));
    commandLineParser.process(app);
    QStringList positionalArguments = commandLineParser.positionalArguments();

    QUrl url;
    if (positionalArguments.size() > 1) {
        showHelp(commandLineParser, QStringLiteral("Too many arguments."));
        return -1;
    } else if (positionalArguments.size() == 1)
        url = QUrl::fromUserInput(positionalArguments.at(0));
    else
        url = QUrl(QStringLiteral("http://www.baidu.com"));

    if (!url.isValid()) {
        showHelp(commandLineParser, QString("%1 is not a valid url.").arg(positionalArguments.at(0)));
        return -1;
    }
	*/
//	QUrl url = QUrl(QStringLiteral("https://www.baidu.com"));
//    MainWindow browser(url);
//    browser.show();

	QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
	//	db.setDatabaseName(":memory:");
	db.setDatabaseName("calllog.db");
	db.open();

	QSqlQuery query;
	query.exec("create table CallLog(callnumber varchar(32), calltype varchar(16), starttime int, duration int, calllocation varchar(128), otherCallLocation varchar(32), callInLac varchar(32), callInCellid varchar(32))");

	/*
	QSqlQuery query;
	QString cmd;
	QTextStream(&cmd) << "insert into CallLog values("
	<< ", '" << callNumber << "'"
	<< ", '" << callType << "'"
	<< (int)QDateTime::fromString(startTime, "yyyy-MM-dd hh:mm:ss").toTime_t()
	<< ", " << duration.msecsSinceStartOfDay()
	<< ", '" << callLocation << "'"
	<< ", '" << otherCallLocation << "'"
	<< ", '" << callInLac << "'"
	<< ", '" << callInCellid << "'"
	<< ")";
	query.exec(cmd);
	*/
	ChartDialog viewer;
	viewer.show();
    return app.exec();
}
