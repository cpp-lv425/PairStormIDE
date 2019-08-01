#include "documentationviewer.h"

#include <QMenu>
#include <QToolTip>
#include <QToolBar>
#include <QMenuBar>
#include <QLineEdit>
#include <QStatusBar>
#include <QProgressBar>
#include <QWebEngineView>
#include <QWebEnginePage>
#include <QStackedWidget>
#include <QWebEngineHistory>

DocumentationViewer::DocumentationViewer(QWidget *parent)
    : QMainWindow (parent)
{
   // setupUI();
    mUrlEdit = new QLineEdit(this);
    mProgressBar = new QProgressBar(this);
    mWebView = new QWebEngineView(this);
    mWebView->load(QUrl("http://www.google.com"));
    setCentralWidget(mWebView);
    mStatusBar = new QStatusBar(this);

    mBottomToolBar = new QToolBar(this);
    addToolBar(Qt::BottomToolBarArea, mBottomToolBar);

    mStackedWidget = new QStackedWidget(this);

    setWindowTitle("C++ Documentation");
    resize(800, 600);

    QMenu *pViewMenu = new QMenu("&View",this);
    pViewMenu->addAction("&History", this, &DocumentationViewer::onHistoryTriggered, Qt::CTRL + Qt::Key_H);
    pViewMenu->addAction("&Exit", this, &DocumentationViewer::onExitTrigerred, Qt::ALT + Qt::Key_F4);

    QMenu *pPageMenu = new QMenu("&Page",this);
    mNextPageAction = pPageMenu->addAction("&Next",mWebView, &QWebEngineView::forward);
    mPrevPageAction = pPageMenu->addAction("&Prev", mWebView, &QWebEngineView::back);
    mRefreshPageAction = pPageMenu->addAction("&Refresh", mWebView, &QWebEngineView::reload);
    mStopLoadAction = pPageMenu->addAction("&Stop", mWebView, &QWebEngineView::stop);

    mNextPageAction->setIcon(QIcon::fromTheme("go-next"));
    mPrevPageAction->setIcon(QIcon::fromTheme("go-previous"));
    mRefreshPageAction->setIcon(QIcon::fromTheme("view-refresh"));
    mStopLoadAction->setIcon(QIcon::fromTheme("process-stop"));


    QToolBar *pToolBar = addToolBar("Main Tool Bar");;
    pToolBar->addAction(mPrevPageAction);
    pToolBar->addAction(mNextPageAction);
    pToolBar->addAction(mRefreshPageAction);
    pToolBar->addAction(mStopLoadAction);
    pToolBar->addWidget(mUrlEdit);

    menuBar()->addMenu(pViewMenu);
    menuBar()->addMenu(pPageMenu);

    connect(mWebView, &QWebEngineView::urlChanged, this, &DocumentationViewer::updateUrlBar);
    connect(mUrlEdit, &QLineEdit::returnPressed, this, &DocumentationViewer::urlRequested);
    connect(mWebView, &QWebEngineView::titleChanged, this, &DocumentationViewer::titleChange);

    mStatusBar = statusBar();


    pToolBar->addWidget(mStackedWidget);
    mStackedWidget->addWidget(mUrlEdit);
    mStackedWidget->addWidget(mProgressBar);

    mStackedWidget->setCurrentWidget(mUrlEdit);
    connect(mWebView, &QWebEngineView::loadStarted, this, &DocumentationViewer::loadStarted);
    connect(mWebView, &QWebEngineView::loadFinished, this, &DocumentationViewer::loadFinished);
    connect(mWebView, &QWebEngineView::loadProgress, mProgressBar, &QProgressBar::setValue);
    //connect(mWebView, SIGNAL(iconChanged()), this, SLOT(iconChanged()));
    setWindowIcon(mWebView->icon());

    mCurrentPage = mWebView->page();

    addToolBar(Qt::TopToolBarArea, pToolBar);

    connect(mCurrentPage, &QWebEnginePage::linkHovered, this, &DocumentationViewer::linkHovered);

}

void DocumentationViewer::setupUI()
{


}

void DocumentationViewer::updateUrlBar(const QUrl &url)
{
    mCurrentPage = mWebView->page();
    mUrlEdit->setText(url.toString()); //change from QUrl to QString
    mUrlEdit->setCursorPosition(0);
}

void DocumentationViewer::urlRequested() {
    QString url = mUrlEdit->text();
    if (!url.startsWith("http://")) {
        url.push_front("http://");
    }
    mWebView->load(QUrl(url));
}

void DocumentationViewer::titleChange(const QString &title) {
    setWindowTitle(title + " - Browser");
}

void DocumentationViewer::loadStarted()
{
    mStackedWidget->setCurrentWidget(mProgressBar);
    mStopLoadAction->setDisabled(false);
    mStatusBar->showMessage(mUrlEdit->text());
}

void DocumentationViewer::loadFinished(bool ok)
{
    qDebug()<<"OK";
    mStackedWidget->setCurrentWidget(mUrlEdit);
    mStopLoadAction->setDisabled(true);
    mPrevPageAction->setDisabled(!mWebView->history()->canGoBack());
    mNextPageAction->setDisabled(!mWebView->history()->canGoForward());
    mStatusBar->clearMessage();
}

void DocumentationViewer::iconChanged()
{
    setWindowIcon(mWebView->page()->icon());
}

void DocumentationViewer::linkHovered(const QString &url)
{
    QToolTip::showText(cursor().pos(),url,this, rect() );
}

void DocumentationViewer::onHistoryTriggered()
{
    qDebug()<<"Ok";
}

void DocumentationViewer::onHelpTrigerred()
{
    qDebug()<<"Ok";
}

void DocumentationViewer::onExitTrigerred()
{
    qDebug()<<"Ok";
}

//void Browser::find() {
//    if (web_view->findText(phrase->text(),web_view->page()->findText(phrase->text()))) {
//		status_bar->showMessage("Found: \"" + phrase->text() + "\"");
//	} else {
//		status_bar->showMessage("Not found: \"" + phrase->text() + "\"");
//	}
//}
