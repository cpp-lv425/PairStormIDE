#include "browser.h"

#include <QDir>
#include <QSizePolicy>
#include <QMdiSubWindow>
#include <QStandardPaths>
#include <QWebEngineView>
#include <QWebEnginePage>

#include "mdiarea.h"
#include "documentationsearch.h"
#include "connectionmanager.h"
#include "documentationengine.h"
#include "documentationviewer.h"
#include "htmlcontentgenerator.h"

Browser::Browser(QWidget *parent) : QMainWindow (parent)
{
    mConnectionManager = new ConnectionManager(this);
    mDocumentationEngine = new DocumentationEngine(this);
    mBrowseArea = new MDIArea;
    mBrowseArea->setViewMode(QMdiArea::ViewMode::TabbedView);
    setCentralWidget(mBrowseArea);
}

Browser::~Browser()
{
    delete mConnectionManager;
    delete mDocumentationEngine;
    delete mBrowseArea;
    for(auto &a : mBrowseArea->subWindowList())
    {
        delete a;
    }
    qDebug()<<"Delete browser";

    bool result;
    QDir dir = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);

    result = dir.cd("PairStorm");
    result = dir.cd("temp");

    if(!result)
    {
        return;
    }

    dir.setNameFilters(QStringList() << "*.html");
    dir.setFilter(QDir::Files);

    foreach(QString dirFile, dir.entryList())
    {
        dir.remove(dirFile);
    }
}

void Browser::newTab(const QString &keyword)
{
    DocumentationViewer *newWindow = new DocumentationViewer(this);

    bool isPairStormExist;
    QDir dir = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);

    isPairStormExist = dir.cd("PairStorm");

    if(!isPairStormExist)
    {
        if(mConnectionManager->hasConnection())
        {
            newWindow->webView()->load(QUrl("https://en.cppreference.com/w/"));
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
            newWindow->webView()->load(QUrl::fromLocalFile(QString(tempDir.path()+"/"+keyword+".html")));
        }
        else
        {
            if(mConnectionManager->hasConnection())
            {
                newWindow->webView()->load(QUrl("https://en.cppreference.com/w/"));
            }
            else
            {
                dir.cd("reference");
                dir.cd("en");
                QString indexPath{dir.path() + "/" + "index.html"};
                qDebug()<<indexPath;
                newWindow->webView()->load(QUrl::fromLocalFile(indexPath));
            }
        }
    }
    mBrowseArea->addSubWindow(newWindow);
    newWindow->setAttribute(Qt::WA_DeleteOnClose);

}

void Browser::emptyDocumentationTab()
{
    DocumentationViewer *newWindow = new DocumentationViewer(mBrowseArea);
    bool isPairStormExist;
    QDir dir = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);

    isPairStormExist = dir.cd("PairStorm");

    if(mConnectionManager->hasConnection())
    {
        newWindow->webView()->load(QUrl("https://en.cppreference.com/w/"));
    }
    else
    {
        dir.cd("reference");
        dir.cd("en");
        QString indexPath{dir.path() + "/" + "index.html"};
        qDebug()<<indexPath;
        newWindow->webView()->load(QUrl::fromLocalFile(indexPath));
    }

    qDebug()<<"HERE";
    auto temp = mBrowseArea->addSubWindow(newWindow);
    temp->setWindowState(Qt::WindowState::WindowMaximized);
    newWindow->setAttribute(Qt::WA_DeleteOnClose);
}

