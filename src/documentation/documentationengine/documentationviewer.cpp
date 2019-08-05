#include "documentationviewer.h"

#include <QDir>
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
#include <QStandardPaths>
#include <QWebEngineHistory>

#include "urlglobal.h"
#include "connectionmanager.h"
#include "documentationengine.h"
#include "documentationsearch.h"
#include "htmlcontentgenerator.h"

DocumentationViewer::DocumentationViewer(QWidget *parent)
    : QMainWindow (parent)
{
    mDocumentationEngine = new DocumentationEngine(this);
    mConnectionManager = new ConnectionManager(this);

    mUrlEdit = new QLineEdit(this);
    mProgressBar = new QProgressBar(this);
    mWebView = new QWebEngineView(this);
    mWebView->load(ReferenceUrl::google);
    setCentralWidget(mWebView);

    mBottomToolBar = new QToolBar(this);
    addToolBar(Qt::BottomToolBarArea, mBottomToolBar);

    mStackedWidget = new QStackedWidget(this);

    setWindowTitle("C++ Documentation");

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

void DocumentationViewer::loadReferenceDocumentation()
{
    bool isPairStormExist;
    QDir dir = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);

    isPairStormExist = dir.cd("PairStorm");

    if(mConnectionManager->hasConnection())
    {
        mWebView->load(ReferenceUrl::reference);
    }
    else
    {
        dir.cd("reference");
        dir.cd("en");
        QString indexPath{dir.path() + "/" + "index.html"};
        mWebView->load(QUrl::fromLocalFile(indexPath));
    }
}

void DocumentationViewer::loadReferenceDocumentation(const QString &keyword)
{
    bool isPairStormExist;
    QDir dir = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);

    isPairStormExist = dir.cd("PairStorm");

    if(!isPairStormExist)
    {
        if(mConnectionManager->hasConnection())
        {
            mWebView->load(ReferenceUrl::reference);
        }
    }
    else
    {
        dir.mkdir("temp");
        QDir tempDir{dir};
        tempDir.cd("temp");
        mDocumentationEngine->searchByKeyword(keyword);
        QString html = QString::fromStdString(HTMLContentGenerator::generate(mDocumentationEngine->documentationFiles()));

        if(mDocumentationEngine->documentationFiles().size() > 0)
        {
            QFile temp(QString(tempDir.path()+"/"+keyword+".html"));

            temp.open(QIODevice::WriteOnly);
            temp.write(html.toUtf8());

            temp.close();
            webView()->load(QUrl::fromLocalFile(QString(tempDir.path()+"/"+keyword+".html")));
        }
        else
        {
            if(mConnectionManager->hasConnection())
            {
                webView()->load(ReferenceUrl::reference);
            }
            else
            {
                dir.cd("reference");
                dir.cd("en");
                QString indexPath{dir.path() + "/" + "index.html"};
                webView()->load(QUrl::fromLocalFile(indexPath));
            }
        }
    }
}

void DocumentationViewer::updateUrlBar(const QUrl &url)
{
    mCurrentPage = mWebView->page();
    mUrlEdit->setText(url.toString());
    mUrlEdit->setCursorPosition(0);
}

void DocumentationViewer::urlRequested()
{
    QString stringUrl = mUrlEdit->text();
    QUrl tempUrl{stringUrl};
    if(tempUrl.isValid())
    {
        mWebView->load(QUrl(tempUrl));
    }
    else
    {
        mWebView->load(ReferenceUrl::google);
    }
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
    qDebug()<<"History";
}

void DocumentationViewer::onHelpTrigerred()
{
    qDebug()<<"Help";
}

void DocumentationViewer::onExitTrigerred()
{
    qDebug()<<"Exit";
}
