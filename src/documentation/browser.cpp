#include "browser.h"

#include "mdiarea.h"
#include "connectionmanager.h"
#include "documentationengine.h"
#include "documentationviewer.h"
#include "htmlcontentgenerator.h"
#include <QWebEngineView>
#include <QWebEnginePage>
#include <QDebug>
#include <QWebEngineSettings>
#include <QMessageBox>
#include <QDir>

Browser::Browser(QWidget *parent) : QMainWindow (parent)
{
    mConnectionManager = new ConnectionManager(this);
    mDocumentationEngine = new DocumentationEngine(this);
    mBrowseArea = new MDIArea(this);
    setCentralWidget(mBrowseArea);
}

void Browser::newTab(const QString &keyword)
{
    DocumentationViewer *newWingow = new DocumentationViewer(this);
    mDocumentationEngine->searchByKeyword(keyword);
    QString html = QString::fromStdString(HTMLContentGenerator::generate(mDocumentationEngine->documentationLinks()));

    if(mDocumentationEngine->documentationLinks().size() > 0)
    {
        QFile temp(QString(QDir::home().path()+"/temp.html"));
        temp.open(QIODevice::WriteOnly);
        temp.write(html.toUtf8());
        temp.close();
        newWingow->setHtml(html);
    }

    newWingow->webView()->load(QUrl::fromLocalFile(QString(QDir::home().path()+"/temp.html")));
    mBrowseArea->addSubWindow(newWingow);
    newWingow->setAttribute(Qt::WA_DeleteOnClose);

}

void Browser::emptyDocumentationTab()
{


}

