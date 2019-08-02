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
    mUrlEdit = new QLineEdit(this);
    mProgressBar = new QProgressBar(this);
    mWebView = new QWebEngineView(this);
    mWebView->load(QUrl("http://www.google.com"));
    setCentralWidget(mWebView);

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


    pToolBar->addWidget(mStackedWidget);
    mStackedWidget->addWidget(mUrlEdit);
    mStackedWidget->addWidget(mProgressBar);

    mStackedWidget->setCurrentWidget(mUrlEdit);
    connect(mWebView, &QWebEngineView::loadStarted, this, &DocumentationViewer::loadStarted);
    connect(mWebView, &QWebEngineView::loadFinished, this, &DocumentationViewer::loadFinished);
    connect(mWebView, &QWebEngineView::loadProgress, mProgressBar, &QProgressBar::setValue);
    setWindowIcon(mWebView->icon());

    mCurrentPage = mWebView->page();

    addToolBar(Qt::TopToolBarArea, pToolBar);

    connect(mCurrentPage, &QWebEnginePage::linkHovered, this, &DocumentationViewer::linkHovered);

}

QWebEngineView *DocumentationViewer::webView() const
{
    return mWebView;
}

void DocumentationViewer::setHtml(const QString &html)
{
    mWebView->setHtml(html);
}

void DocumentationViewer::updateUrlBar(const QUrl &url)
{
    mCurrentPage = mWebView->page();
    mUrlEdit->setText(url.toString());
    mUrlEdit->setCursorPosition(0);
}

void DocumentationViewer::urlRequested() {
    QString url = mUrlEdit->text();
    mWebView->load(QUrl(url));
}

void DocumentationViewer::titleChange(const QString &title) {
    setWindowTitle(title);
}

void DocumentationViewer::loadStarted()
{
    mStackedWidget->setCurrentWidget(mProgressBar);
    mStopLoadAction->setDisabled(false);
}

void DocumentationViewer::loadFinished(bool ok)
{
    mStackedWidget->setCurrentWidget(mUrlEdit);
    mStopLoadAction->setDisabled(ok);
    mPrevPageAction->setDisabled(!mWebView->history()->canGoBack());
    mNextPageAction->setDisabled(!mWebView->history()->canGoForward());
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