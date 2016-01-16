#ifndef WINDOW_BROWSER_H
#define WINDOW_BROWSER_H

#include <QMainWindow>
#include <QWebElement>
#include <QLineEdit>
#include "ui_WindowBrowser.h"

class WebView;

class WindowBrowser : public QMainWindow
{
    Q_OBJECT

public:
	WindowBrowser(QWidget * parent = 0, Qt::WindowFlags flags = 0);

signals:
	void signalCallLogChanged();

protected slots:
	void slotAdjustTitle();
	void slotAdjustProgress(int progress);
	void slotGotoLinkUrl(const QUrl & url);
	void slotChangeLocation();
	void slotFetchCallLog();

private:
	//���й����Źٷ���վ��ȡͨ����¼
	void fetchCallLogFromChinaTelecomTable(QWebElement& table);

	//���й��ƶ�ͨ���굥��ȡͨ����¼
	QWebElement findTableFromChinaMobileWebFrame(QWebFrame* webFrame);
	QWebElement const& findTableFromChinaMobileWebTable(QWebElement const& table);
	void fetchCallLogFromChinaMobileTable(QWebElement& table);

private:
	Ui::WindowBrowser ui;

    WebView* m_webView;
	QLineEdit* m_locationEdit;
};

#endif