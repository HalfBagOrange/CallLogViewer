#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMessageBox>
#include <QWebFrame>
#include <QWebElement>
#include <QWebView>
#include <QNetworkProxyFactory>

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
		return this;
	}
};

WindowBrowser::WindowBrowser(QWidget * parent, Qt::WindowFlags flags)
:QMainWindow(parent, flags)
{
	ui.setupUi(this);

	setWindowTitle(QStringLiteral("��HTML��������...."));
	setWindowState(Qt::WindowMaximized);

    QNetworkProxyFactory::setUseSystemConfiguration(true);

	m_webView = new WebView(this);
	m_webView->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
	setCentralWidget(m_webView);

	ui.label->setParent(m_webView);

	connect(m_webView, SIGNAL(titleChanged(QString)), SLOT(slotAdjustTitle()));
	connect(m_webView, SIGNAL(loadProgress(int)), SLOT(slotAdjustProgress(int)));
	connect(m_webView, SIGNAL(linkClicked(const QUrl&)), SLOT(slotGotoLinkUrl(const QUrl&)));

	QAction* act = m_webView->pageAction(QWebPage::Back);
	act->setText(QStringLiteral("����"));
	ui.toolBar->addAction(act);

	act = m_webView->pageAction(QWebPage::Forward);
	act->setText(QStringLiteral("ǰ��"));
	ui.toolBar->addAction(act);

	act = m_webView->pageAction(QWebPage::Reload);
	act->setText(QStringLiteral("����"));
	ui.toolBar->addAction(act);

	act = m_webView->pageAction(QWebPage::Stop);
	act->setText(QStringLiteral("ֹͣ"));
	ui.toolBar->addAction(act);

	act = new QAction(QIcon("fetch.png"), QStringLiteral("��ȡͨ����¼"), this);
	connect(act, SIGNAL(triggered()), this, SLOT(slotFetchCallLog()));
	ui.toolBar->addAction(act);

	m_locationEdit = new QLineEdit(this);
	m_locationEdit->setSizePolicy(QSizePolicy::Expanding, m_locationEdit->sizePolicy().verticalPolicy());
	connect(m_locationEdit, SIGNAL(returnPressed()), SLOT(slotChangeLocation()));
	ui.toolBar->addWidget(m_locationEdit);
}

void WindowBrowser::slotAdjustTitle()
{
	QString text = m_webView->title();
	if (text.isEmpty())
	{
		text = "CallLogBrowser";
	}
	setWindowTitle(text);
}

void WindowBrowser::slotAdjustProgress(int progress)
{
	QString text = m_webView->title();
	if (text.isEmpty())
	{
		text = "CallLogBrowser";
	}
	if (progress <= 0 || progress >= 100)
	{
		setWindowTitle(text);
	}
	else
	{
		setWindowTitle(QString("%1 (%2%)").arg(text).arg(progress));
	}
}

void WindowBrowser::slotGotoLinkUrl(const QUrl& url)
{
    m_locationEdit->setText(url.toString());
	m_webView->load(url);
}

void WindowBrowser::slotChangeLocation()
{
	QUrl url = QUrl::fromUserInput(m_locationEdit->text());
	m_webView->load(url);
	m_webView->setFocus();
}

void WindowBrowser::slotFetchCallLog()
{
	QWebFrame* webFrame = m_webView->page()->mainFrame();
	if (!webFrame)
	{
		return;
	}

	// ���й����Źٷ���ҳ��ȡͨ����¼
	QWebElement& chinaTelecomTable = webFrame->findFirstElement("#Pzone_details_content_2");
	if (!chinaTelecomTable.isNull())
	{
		fetchCallLogFromChinaTelecomTable(chinaTelecomTable);
		return;
	}
	
	//���й��ƶ�ͨ���굥����ȡͨ����¼
	QWebElement chinaMobileTable = findTableFromChinaMobileWebFrame(webFrame);
	if (!chinaMobileTable.isNull())
	{
		fetchCallLogFromChinaMobileTable(chinaMobileTable);
		return;
	}

	QMessageBox::about(this, QStringLiteral("��Ϣ"), QStringLiteral("��ȡͨ����¼ʧ�ܣ�"));
}

QWebElement WindowBrowser::findTableFromChinaMobileWebFrame(QWebFrame* webFrame)
{
	QWebElementCollection tableCollection = webFrame->findAllElements("table");
	for (int i = 0; i < tableCollection.count(); i++)
	{
		QWebElement table = findTableFromChinaMobileWebTable(tableCollection[i]);
		if (!table.isNull())
		{
			return table;
		}
	}
	return QWebElement();
}

QWebElement const& WindowBrowser::findTableFromChinaMobileWebTable(QWebElement const& table)
{
	QWebElementCollection& tableCollection = table.findAll("table");
	for (int i = 0; i < tableCollection.count(); i++)
	{
		QWebElement table1 = findTableFromChinaMobileWebTable(tableCollection[i]);
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

void WindowBrowser::fetchCallLogFromChinaMobileTable(QWebElement& table)
{
	if (QMessageBox::No == QMessageBox::question(this, QStringLiteral("ѯ��"), QStringLiteral("�Ƿ�ִ����ȡ���ݲ�����������̱ȽϺ�ʱ")))
	{
		return;
	}
	if (QMessageBox::Yes == QMessageBox::question(this, QStringLiteral("ѯ��"), QStringLiteral("�Ƿ�ɾ��ԭ�ȵ�����")))
	{
		QSqlQuery query;
		query.exec("delete from CallLog");
	}
	QMessageBox::about(this, QStringLiteral("��Ϣ"), QStringLiteral("��ʼ��ȡͨ����¼���˹��̱ȽϺ�ʱ�����ĵȴ�"));

	//setDisabled(true);
	setCursor(Qt::BusyCursor);
	ui.label->resize(m_webView->size());
	ui.label->setText(QStringLiteral("ϵͳ��æ��������ȡ����,�����ĵȴ�......"));
	ui.label->repaint();
	repaint();

	int count = 0;
	QWebElement & tbody = table.findFirst("tbody");
	QWebElementCollection & trCollection = tbody.findAll("tr");
	int trCount = trCollection.count();
	for (int trIndex = 0; trIndex < trCount; trIndex++)
	{
		QWebElement & tr = trCollection[trIndex];
		if (tr.classes()[0] == "listtitle")
		{
			continue;
		}

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
		QWebElementCollection & tdCollection = tr.findAll("td");
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

		QString hour,minute,second;
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

	//setEnabled(true);
	unsetCursor();
	ui.label->setText("");
	ui.label->repaint();
	repaint();

	QString info;
	QTextStream(&info) << QStringLiteral("��ȡ") << count << QStringLiteral("��ͨ����¼���뼰ʱ�鿴");
	QMessageBox::about(this, QStringLiteral("��Ϣ"), info);

	emit signalCallLogChanged();
}

void WindowBrowser::fetchCallLogFromChinaTelecomTable(QWebElement& table)
{
	if (QMessageBox::No == QMessageBox::question(this, QStringLiteral("ѯ��"), QStringLiteral("�Ƿ�ִ����ȡ���ݲ�����������̱ȽϺ�ʱ")))
	{
		return;
	}
	if (QMessageBox::Yes == QMessageBox::question(this, QStringLiteral("ѯ��"), QStringLiteral("�Ƿ�ɾ��ԭ�ȵ�����")))
	{
		QSqlQuery query;
		query.exec("delete from CallLog");
	}
	QMessageBox::about(this, QStringLiteral("��Ϣ"), QStringLiteral("��ʼ��ȡͨ����¼���˹��̱ȽϺ�ʱ�����ĵȴ�"));

	//setDisabled(true);
	setCursor(Qt::BusyCursor);
	ui.label->resize(m_webView->size());
	ui.label->setText(QStringLiteral("ϵͳ��æ��������ȡ����,�����ĵȴ�......"));
	ui.label->repaint();
	repaint();

    QWebElementCollection trAll = table.findAll("tr");
    qDebug() << "tr count " << trAll.count();

    int count = 0;
    for(int i=0; i < trAll.count(); i++)
    {
        QWebElement& tr = trAll[i];
        QWebElementCollection tdAll = tr.findAll("td");
        qDebug() << "tr index " << i << " td count " << tdAll.count();

		//ͨ����¼��12��
        if(tdAll.count() != 12)
        {
			continue;
        }

		//��һ���Ǳ���
		if (count++ <= 0)
		{
			continue;
		}

		QString index = tdAll[0].toPlainText();
		QString number = tdAll[1].toPlainText();
		QString type = tdAll[2].toPlainText();
		QString time = tdAll[3].toPlainText();
		QString duration = tdAll[4].toPlainText();
		QString address = tdAll[5].toPlainText();
		QString chargeType = tdAll[6].toPlainText();
		QString localCharge = tdAll[7].toPlainText();
		QString foreignerCharge = tdAll[8].toPlainText();
		QString freeCharge = tdAll[9].toPlainText();
		QString totalCharge = tdAll[10].toPlainText();
		QString remarks = tdAll[11].toPlainText();

		QSqlQuery query;
		QString cmd;
		QTextStream(&cmd) << "insert into CallLog values("
			<< "'" << number << "'"
			<< ", '" << type << "'"
			<< ", " << (int)QDateTime::fromString(time, "yyyy-MM-dd hh:mm:ss").toTime_t() 
			<< ", " << QTime::fromString(duration, "hh:mm:ss").msecsSinceStartOfDay() 
			<< ", '" << address << "', ');";
		query.exec(cmd);
    }

	//setEnabled(true);
	unsetCursor();
	ui.label->setText("");
	ui.label->repaint();
	repaint();

    QString info;
    QTextStream(&info) << QStringLiteral("��ȡ") << count << QStringLiteral("��ͨ����¼���뼰ʱ�鿴");
    QMessageBox::about(this, QStringLiteral("��Ϣ"), info);

	emit signalCallLogChanged();
}
