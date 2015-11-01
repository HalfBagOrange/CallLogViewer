#include <QtWidgets>
#include <QtNetwork>
#include <QtWebKitWidgets>
#include <QSqlDatabase>
#include <QSqlQuery>

#include "BrowserMainWindow.h"


BrowserMainWindow::BrowserMainWindow(QWidget *parent, Qt::WindowFlags flags)
: QMainWindow(parent, flags)
{
	setWindowState(Qt::WindowMaximized);
    QNetworkProxyFactory::setUseSystemConfiguration(true);

//! [2]
    view = new QWebView(this);
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
	act->setText(QStringLiteral("后退"));
	toolBar->addAction(act);
	act = view->pageAction(QWebPage::Forward);
	act->setText(QStringLiteral("前进"));
	toolBar->addAction(act);
	act = view->pageAction(QWebPage::Reload);
	act->setText(QStringLiteral("重载"));
	toolBar->addAction(act);
	act = view->pageAction(QWebPage::Stop);
	act->setText(QStringLiteral("停止"));
	toolBar->addAction(act);
    toolBar->addWidget(locationEdit);

	act = new QAction(this);
	QIcon icon;
	icon.addFile("fetch.png");
	act->setIcon(icon);
	act->setText(QStringLiteral("提取通话记录"));
	connect(act, SIGNAL(triggered()), this, SLOT(fetchCallLog()));
	toolBar->addAction(act);
//! [2]

	/*
    QMenu *viewMenu = menuBar()->addMenu(QStringLiteral("开发"));
    QAction* viewSourceAction = new QAction(QStringLiteral("Html源码"), this);
    connect(viewSourceAction, SIGNAL(triggered()), SLOT(viewSource()));
    viewMenu->addAction(viewSourceAction);
	*/
//! [3]
/*
    QMenu *effectMenu = menuBar()->addMenu(tr("&Effect"));
    effectMenu->addAction("Highlight all links", this, SLOT(highlightAllLinks()));

    rotateAction = new QAction(this);
    rotateAction->setIcon(style()->standardIcon(QStyle::SP_FileDialogDetailedView));
    rotateAction->setCheckable(true);
    rotateAction->setText(tr("Turn images upside down"));
    connect(rotateAction, SIGNAL(toggled(bool)), this, SLOT(rotateImages(bool)));
    effectMenu->addAction(rotateAction);
*/
	/*
    QMenu *toolsMenu = menuBar()->addMenu(QStringLiteral("工具"));
    toolsMenu->addAction(QStringLiteral("提取通话记录"), this, SLOT(fetchCallLog()));
    toolsMenu->addAction(QStringLiteral("通话记录"), this, SLOT(callLogTable()));
	*/
/*
    toolsMenu->addAction(tr("Remove GIF images"), this, SLOT(removeGifImages()));
    toolsMenu->addAction(tr("Remove all inline frames"), this, SLOT(removeInlineFrames()));
    toolsMenu->addAction(tr("Remove all object elements"), this, SLOT(removeObjectElements()));
    toolsMenu->addAction(tr("Remove all embedded elements"), this, SLOT(removeEmbeddedElements()));
*/
    setCentralWidget(view);
}

BrowserMainWindow::~BrowserMainWindow()
{
}

void BrowserMainWindow::gotoLinkUrl(const QUrl& url)
{
    locationEdit->setText(url.toString());
    view->load(url);
}

// G:\CallLogViewer\清单查询详情-产品专区-中国电信网上营业厅·浙江5.html
void BrowserMainWindow::fetchCallLog()
{
    qDebug() << "start fatch call log";

    QWebFrame* webFrame = view->page()->mainFrame();
    QWebElement table = webFrame->findFirstElement("#Pzone_details_content_2");

    if(table.isNull())
    {
        QMessageBox::about(this, QStringLiteral("提示"), QStringLiteral("找不到通话记录表"));
        return;
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
		time.remove(time.size()/2, time.size()/2);
		QString duration = tdAll[4].toPlainText();
		duration.remove(duration.size()/2, duration.size()/2);
		QString address = tdAll[5].toPlainText();
		QString chargeType = tdAll[6].toPlainText();
		chargeType.remove(chargeType.size() / 2, chargeType.size() / 2);
		QString localCharge = tdAll[7].toPlainText();
		localCharge.remove(localCharge.size() / 2, localCharge.size() / 2);
		QString foreignerCharge = tdAll[8].toPlainText();
		foreignerCharge.remove(foreignerCharge.size() / 2, foreignerCharge.size() / 2);
		QString freeCharge = tdAll[9].toPlainText();
		freeCharge.remove(freeCharge.size() / 2, freeCharge.size() / 2);
		QString totalCharge = tdAll[10].toPlainText();
		totalCharge.remove(totalCharge.size() / 2, totalCharge.size() / 2);
		QString remarks = tdAll[11].toPlainText();
		remarks.remove(remarks.size() / 2, remarks.size() / 2);

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
    QTextStream(&info) << QStringLiteral("提取") << count << QStringLiteral("条通话记录，请及时查看");
    QMessageBox::about(this, QStringLiteral("信息"), info);
}

void BrowserMainWindow::viewSource()
{
    QNetworkAccessManager* accessManager = view->page()->networkAccessManager();
    QNetworkRequest request(view->url());
    QNetworkReply* reply = accessManager->get(request);
    connect(reply, SIGNAL(finished()), this, SLOT(slotSourceDownloaded()));
}

void BrowserMainWindow::slotSourceDownloaded()
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

