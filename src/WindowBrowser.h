#ifndef WINDOW_BROWSER_H
#define WINDOW_BROWSER_H

#include <QMainWindow>
#include <QWebElement>
//class QWebView;
//QT_BEGIN_NAMESPACE
class QLineEdit;
//QT_END_NAMESPACE
class WebView;

class WindowBrowser : public QMainWindow
{
    Q_OBJECT

public:
	WindowBrowser(QWidget * parent = 0, Qt::WindowFlags flags = 0);

protected slots:

    void adjustLocation();
    void changeLocation();
    void adjustTitle();
    void setProgress(int p);
    void finishLoading(bool);

    void viewSource();
    void slotSourceDownloaded();

    void highlightAllLinks();
    void rotateImages(bool invert);
    void removeGifImages();
    void removeInlineFrames();
    void removeObjectElements();
    void removeEmbeddedElements();

    void gotoLinkUrl(const QUrl& url);
    void fetchCallLog();
    void callLogTable();

	QWebElement & findDetailFromFrame(QWebFrame* webFrame);
	QWebElement & findDetailFromTable(QWebElement& table);
	void fetchCallLogFromTelecom(QWebElement& table);
	void fetchCallLogFromTeleDetall(QWebElement& table);
private:
    QString jQuery;
    WebView *view;
    QLineEdit *locationEdit;
    QAction *rotateAction;
    int progress;
};

#endif