#ifndef BROWSER_H
#define BROWSER_H

#include <QtWidgets>

class QWebView;
class QLineEdit;

class BrowserMainWindow : public QMainWindow
{
    Q_OBJECT

public:
	BrowserMainWindow(QWidget *parent = 0, Qt::WindowFlags flags = 0);
	~BrowserMainWindow();

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

private:
    QString jQuery;
    QWebView *view;
    QLineEdit *locationEdit;
    QAction *rotateAction;
    int progress;
//! [1]
};

#endif