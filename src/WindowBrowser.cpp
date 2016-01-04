
#include <QtWidgets>
#include <QtNetwork>
#include <QtWebKitWidgets>
#include <QSqlDatabase>
#include <QSqlQuery>
#include "WindowBrowser.h"

class WebView : public QWebView
{
public : 
	WebView(QWidget *parent)
		:QWebView(parent)
	{
		settings()->setAttribute(QWebSettings::JavascriptCanOpenWindows, true);
		settings()->setDefaultTextEncoding("GBK");
	}

	QWebView *createWindow(QWebPage::WebWindowType type)
	{
		int i = (int)type;
		QWebView *view = QWebView::createWindow(type);
		/*
		if ()
		{

		}
		*/
		return this;
	}
};

MainWindow::MainWindow(const QUrl& url)
{
	setWindowTitle(QStringLiteral("��HTML��������"));
    progress = 0;

	m_firtOpen = true;
	setWindowState(Qt::WindowMaximized);
/*
    QFile file;
    file.setFileName(":/jquery.min.js");
    file.open(QIODevice::ReadOnly);
    jQuery = file.readAll();
    jQuery.append("\nvar qt = { 'jQuery': jQuery.noConflict(true) };");
    file.close();
*/

//! [1]

    QNetworkProxyFactory::setUseSystemConfiguration(true);

//! [2]
    view = new WebView(this);
    view->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
    view->load(url);

//    connect(view, SIGNAL(loadFinished(bool)), SLOT(adjustLocation()));
    connect(view, SIGNAL(titleChanged(QString)), SLOT(adjustTitle()));
    connect(view, SIGNAL(loadProgress(int)), SLOT(setProgress(int)));
    connect(view, SIGNAL(linkClicked(const QUrl&)), SLOT(gotoLinkUrl(const QUrl&)));
//    connect(view, SIGNAL(loadFinished(bool)), SLOT(finishLoading(bool)));

    locationEdit = new QLineEdit(this);
    locationEdit->setSizePolicy(QSizePolicy::Expanding, locationEdit->sizePolicy().verticalPolicy());
    connect(locationEdit, SIGNAL(returnPressed()), SLOT(changeLocation()));
    locationEdit->setText(url.toString());

    QToolBar *toolBar = addToolBar(tr("Navigation"));
	QAction* act = view->pageAction(QWebPage::Back);
	act->setText(QStringLiteral("����"));
	toolBar->addAction(act);
	act = view->pageAction(QWebPage::Forward);
	act->setText(QStringLiteral("ǰ��"));
	toolBar->addAction(act);
	act = view->pageAction(QWebPage::Reload);
	act->setText(QStringLiteral("����"));
	toolBar->addAction(act);
	act = view->pageAction(QWebPage::Stop);
	act->setText(QStringLiteral("ֹͣ"));
	toolBar->addAction(act);
    toolBar->addWidget(locationEdit);

	act = new QAction(this);
	QIcon icon;
	icon.addFile("fetch.png");
	act->setIcon(icon);
	act->setText(QStringLiteral("��ȡͨ����¼"));
	connect(act, SIGNAL(triggered()), this, SLOT(fetchCallLog()));
	toolBar->addAction(act);

    setCentralWidget(view);
    setUnifiedTitleAndToolBarOnMac(true);
}

void MainWindow::gotoLinkUrl(const QUrl& url)
{
    locationEdit->setText(url.toString());
    view->load(url);
}

void MainWindow::fetchCallLog()
{
	qDebug() << "start fatch call log";

	QWebFrame* webFrame = view->page()->mainFrame();

	if (!webFrame)
	{
		return;
	}

	{
		QWebElement& table = webFrame->findFirstElement("#Pzone_details_content_2");
		if (!table.isNull())
		{
			fetchCallLogFromTelecom(table);
			return;
		}
	}
	
	{
		QWebElement table = findDetailFromFrame(webFrame);
		if (!table.isNull())
		{
			fetchCallLogFromTeleDetall(table);
			return;
		}
	}

	QMessageBox::about(this, QStringLiteral("��Ϣ"), QStringLiteral("��ȡͨ����¼ʧ�ܣ�"));
}

QWebElement& MainWindow::findDetailFromFrame(QWebFrame* webFrame)
{
	QWebElementCollection tableCollection = webFrame->findAllElements("table");
	for (int i = 0; i < tableCollection.count(); i++)
	{
		QWebElement table = findDetailFromTable(tableCollection[i]);
		if (!table.isNull())
		{
			return table;
		}
	}
	return QWebElement();
}

QWebElement& MainWindow::findDetailFromTable(QWebElement& table)
{
	QWebElementCollection& tableCollection = table.findAll("table");
	for (int i = 0; i < tableCollection.count(); i++)
	{
		QWebElement table1 = findDetailFromTable(tableCollection[i]);
		if (!table1.isNull())
		{
			return table1;
		}
	}

	QWebElement & tbody = table.findFirst("tbody");
	if (tbody.isNull())
	{
		return QWebElement();
	}

	QWebElementCollection & trCollection = tbody.findAll("tr");
	if (0 == trCollection.count())
	{
		return QWebElement();
	}

	QWebElement & tr = trCollection[0];
	QWebElementCollection & tdCollection = tr.findAll("td");
	if (17 != tdCollection.count())
	{
		return QWebElement();
	}

	return table;
}

void MainWindow::fetchCallLogFromTeleDetall(QWebElement& table)
{
//	setDisabled(true);
	if ((QMessageBox::Yes == QMessageBox::question(this, QStringLiteral("ѯ��"), QStringLiteral("�Ƿ�ɾ��ԭ�ȵ�����"))))
	{
		QSqlQuery query;
		query.exec("delete from CallLog");
	}
	QMessageBox::about(this, QStringLiteral("��Ϣ"), QStringLiteral("��ʼ��ȡͨ����¼���˹��̱ȽϺ�ʱ�����ĵȴ�"));

	QWebElement & tbody = table.findFirst("tbody");

	int count = 0;
	QWebElementCollection & trCollection = tbody.findAll("tr");
	int trCount = trCollection.count();
	for (int trIndex = 0; trIndex < trCount; trIndex++)
	{
		QWebElement & tr = trCollection[trIndex];
		if (tr.classes()[0] == "listtitle")
		{
			continue;
		}

		QWebElementCollection & tdCollection = tr.findAll("td");
			/*
			<TD class=tl>���</TD>
			<TD class=tl>��ʼʱ��</TD>
			<TD class=tl>ͨ��ʱ��</TD>
			<TD class=tl>ͨ��״̬</TD>
			<TD class=tl>ͨ������</TD>
			<TD class=tl>�Է�����</TD>
			<TD class=tl>�Է���������</TD>
			<TD class=tl>ͨ����</TD>
			<TD class=tl>ҵ������</TD>
			<TD class=tl>���ػ�����/���η�</TD>
			<TD class=tl>��;��</TD>
			<TD class=tl>��Ϣ��</TD>
			<TD class=tl>����С��</TD>
			<TD class=tl>�Ż���</TD>
			<TD class=tl>�Է�ͨ�����ڵ�</TD>
			<TD class=tl>ͨ������LAC</TD>
			<TD class=tl>ͨ������CELLID</TD>
			*/
			QString index = tdCollection[0].toPlainText().trimmed();
			QString startTime = tdCollection[1].toPlainText().trimmed();
			QString callDuration = tdCollection[2].toPlainText().trimmed();
			QString callStatus = tdCollection[3].toPlainText().trimmed();
			QString callType = tdCollection[4].toPlainText().trimmed();
			QString callNumber = tdCollection[5].toPlainText().trimmed();
			QString callNumberType = tdCollection[6].toPlainText().trimmed();
			QString callLocation = tdCollection[7].toPlainText().trimmed();
			QString businessName = tdCollection[8].toPlainText().trimmed();
			QString localBasicFees = tdCollection[9].toPlainText().trimmed();
			QString longDistanceFees = tdCollection[10].toPlainText().trimmed();
			QString informationFees = tdCollection[11].toPlainText().trimmed();
			QString totalFees = tdCollection[12].toPlainText().trimmed();
			QString discountItems = tdCollection[13].toPlainText().trimmed();
			QString otherCallLocation = tdCollection[14].toPlainText().trimmed();
			QString callInLac = tdCollection[15].toPlainText().trimmed();
			QString callInCellid = tdCollection[16].toPlainText().trimmed();

			int hourPos = callDuration.indexOf("ʱ");
			int minutePos = callDuration.indexOf("��");
			int secondPos = callDuration.indexOf(QStringLiteral("��"));
			QString hour;
			QString minute;
			QString second;
			if (callDuration.contains(QStringLiteral("ʱ")))
			{
				hour = callDuration.section(QStringLiteral("ʱ"), 0, 0);
				callDuration = callDuration.section(QStringLiteral("ʱ"), 1);
			}
			if (callDuration.contains(QStringLiteral("��")))
			{
				minute = callDuration.section(QStringLiteral("��"), 0, 0);
				callDuration = callDuration.section(QStringLiteral("��"), 1);
			}
			if (callDuration.contains(QStringLiteral("��")))
			{
				second = callDuration.section(QStringLiteral("��"), 0, 0);
			}

			QTime duration(hour.toUInt(), minute.toUInt(), second.toUInt());

			/*
			int size = startTime.size();
			startTime.resize(strlen("yyyy-MM-dd hh:mm:ss"));
			int size1 = startTime.size();
			QDateTime strtime = QDateTime::fromString(startTime, "yyyy-MM-dd hh:mm:ss");
			*/
			QSqlQuery query;
			QString cmd;
			QTextStream(&cmd) << "insert into CallLog values("
				<< " '" << callNumber << "'"
				<< ", '" << callType << "'"
				<< ", "<< (int)QDateTime::fromString(startTime, "yyyy-MM-dd hh:mm:ss").toTime_t()
				<< ", " << duration.msecsSinceStartOfDay()
				<< ", '" << callLocation << "'"
				<< ", '" << otherCallLocation << "'"
				<< ", '" << callInLac << "'"
				<< ", '" << callInCellid << "'"
				<< ")";
			query.exec(cmd);
			count++;
	}

//	setDisabled(false);

	QString info;
	QTextStream(&info) << QStringLiteral("��ȡ") << count << QStringLiteral("��ͨ����¼���뼰ʱ�鿴");
	QMessageBox::about(this, QStringLiteral("��Ϣ"), info);

	emit sqlDatachanged();
}

// G:\CallLogViewer\�嵥��ѯ����-��Ʒר��-�й���������Ӫҵ�����㽭5.html
void MainWindow::fetchCallLogFromTelecom(QWebElement& table)
{
    qDebug() << "start fatch call log of chinese telecom";

	if (m_firtOpen && 
		(QMessageBox::Yes == QMessageBox::question(this, QStringLiteral("ѯ��"), QStringLiteral("�Ƿ�ɾ��ԭ�ȵ�����"))))
	{
		m_firtOpen = false;
		QSqlQuery query;
		query.exec("delete from CallLog");
	}

    QWebElementCollection trAll = table.findAll("tr");
    qDebug() << "tr count " << trAll.count();

    int count = 0;
    for(int i=0; i < trAll.count(); i++)
    {
        QWebElement& tr = trAll[i];
        QWebElementCollection tdAll = tr.findAll("td");
        qDebug() << "tr index " << i << " td count " << tdAll.count();

        if(tdAll.count() != 12)
        {
			continue;
        }

		if (count++ <= 0)
		{
			continue;
		}

		QString index = tdAll[0].toPlainText();
		QString number = tdAll[1].toPlainText();
		QString type = tdAll[2].toPlainText();
		QString time = tdAll[3].toPlainText();
//		time.remove(time.size()/2, time.size()/2);
		QString duration = tdAll[4].toPlainText();
//		duration.remove(duration.size()/2, duration.size()/2);
		QString address = tdAll[5].toPlainText();
		QString chargeType = tdAll[6].toPlainText();
//		chargeType.remove(chargeType.size() / 2, chargeType.size() / 2);
		QString localCharge = tdAll[7].toPlainText();
//		localCharge.remove(localCharge.size() / 2, localCharge.size() / 2);
		QString foreignerCharge = tdAll[8].toPlainText();
//		foreignerCharge.remove(foreignerCharge.size() / 2, foreignerCharge.size() / 2);
		QString freeCharge = tdAll[9].toPlainText();
//		freeCharge.remove(freeCharge.size() / 2, freeCharge.size() / 2);
		QString totalCharge = tdAll[10].toPlainText();
//		totalCharge.remove(totalCharge.size() / 2, totalCharge.size() / 2);
		QString remarks = tdAll[11].toPlainText();
//		remarks.remove(remarks.size() / 2, remarks.size() / 2);

		QSqlQuery query;
		QString cmd;
		QTextStream(&cmd) << "insert into CallLog values('" << number << "', '" << type << "', "
			<< (int)QDateTime::fromString(time, "yyyy-MM-dd hh:mm:ss").toTime_t() << ", "
			<< QTime::fromString(duration, "hh:mm:ss").msecsSinceStartOfDay() << ", '" << address << "', '"
			<< chargeType << "', '" << localCharge << "', '" << foreignerCharge << "', '"
			<< freeCharge << "', '" << totalCharge << "', '" << remarks << "');";
		query.exec(cmd);
    }

    QString info;
    QTextStream(&info) << QStringLiteral("��ȡ") << count << QStringLiteral("��ͨ����¼���뼰ʱ�鿴");
    QMessageBox::about(this, QStringLiteral("��Ϣ"), info);

	emit sqlDatachanged();
}

#include "calllogviewer.h"

void MainWindow::callLogTable()
{
	CallLogViewer* viewer = new CallLogViewer(this);
	viewer->show();
}

void MainWindow::viewSource()
{
    QNetworkAccessManager* accessManager = view->page()->networkAccessManager();
    QNetworkRequest request(view->url());
    QNetworkReply* reply = accessManager->get(request);
    connect(reply, SIGNAL(finished()), this, SLOT(slotSourceDownloaded()));
}

void MainWindow::slotSourceDownloaded()
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(const_cast<QObject*>(sender()));
    QTextEdit* textEdit = new QTextEdit(NULL);
    textEdit->setWindowTitle("Html");
    textEdit->setAttribute(Qt::WA_DeleteOnClose);
    textEdit->show();
    textEdit->setPlainText(reply->readAll());
    reply->deleteLater();
}

//! [4]
void MainWindow::adjustLocation()
{
    locationEdit->setText(view->url().toString());
}

void MainWindow::changeLocation()
{
    QUrl url = QUrl::fromUserInput(locationEdit->text());
    view->load(url);
    view->setFocus();
}
//! [4]

//! [5]
void MainWindow::adjustTitle()
{
    if (progress <= 0 || progress >= 100)
        setWindowTitle(view->title());
    else
        setWindowTitle(QString("%1 (%2%)").arg(view->title()).arg(progress));
}

void MainWindow::setProgress(int p)
{
    progress = p;
    adjustTitle();
}
//! [5]

//! [6]
void MainWindow::finishLoading(bool)
{
    progress = 100;
    adjustTitle();
    view->page()->mainFrame()->evaluateJavaScript(jQuery);

    rotateImages(rotateAction->isChecked());
}
//! [6]

//! [7]
void MainWindow::highlightAllLinks()
{
    // We append '; undefined' after the jQuery call here to prevent a possible recursion loop and crash caused by
    // the way the elements returned by the each iterator elements reference each other, which causes problems upon
    // converting them to QVariants.
    QString code = "qt.jQuery('a').each( function () { qt.jQuery(this).css('background-color', 'yellow') } ); undefined";
    view->page()->mainFrame()->evaluateJavaScript(code);
}
//! [7]

//! [8]
void MainWindow::rotateImages(bool invert)
{
    QString code;

    // We append '; undefined' after each of the jQuery calls here to prevent a possible recursion loop and crash caused by
    // the way the elements returned by the each iterator elements reference each other, which causes problems upon
    // converting them to QVariants.
    if (invert)
        code = "qt.jQuery('img').each( function () { qt.jQuery(this).css('-webkit-transition', '-webkit-transform 2s'); qt.jQuery(this).css('-webkit-transform', 'rotate(180deg)') } ); undefined";
    else
        code = "qt.jQuery('img').each( function () { qt.jQuery(this).css('-webkit-transition', '-webkit-transform 2s'); qt.jQuery(this).css('-webkit-transform', 'rotate(0deg)') } ); undefined";
    view->page()->mainFrame()->evaluateJavaScript(code);
}
//! [8]

//! [9]
void MainWindow::removeGifImages()
{
    QString code = "qt.jQuery('[src*=gif]').remove()";
    view->page()->mainFrame()->evaluateJavaScript(code);
}

void MainWindow::removeInlineFrames()
{
    QString code = "qt.jQuery('iframe').remove()";
    view->page()->mainFrame()->evaluateJavaScript(code);
}

void MainWindow::removeObjectElements()
{
    QString code = "qt.jQuery('object').remove()";
    view->page()->mainFrame()->evaluateJavaScript(code);
}

void MainWindow::removeEmbeddedElements()
{
    QString code = "qt.jQuery('embed').remove()";
    view->page()->mainFrame()->evaluateJavaScript(code);
}
//! [9]

