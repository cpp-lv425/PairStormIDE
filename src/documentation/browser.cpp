#include "browser.h"

#include "mdiarea.h"
#include "documentationsearch.h"
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
#include <QStandardPaths>

Browser::Browser(QWidget *parent) : QMainWindow (parent)
{
    mConnectionManager = new ConnectionManager(this);
    mDocumentationEngine = new DocumentationEngine(this);
    mBrowseArea = new MDIArea(this);
    setCentralWidget(mBrowseArea);
}

void Browser::newTab(const QString &keyword)
{
    bool result;
    QDir dir = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    result = dir.cd("PairStorm");
    qDebug()<<result;
    //dir.mkdir("temp");
    qDebug()<<dir.cd("temp");
    //dir.setNameFilters(QStringList() << "*.*");
    //dir.setFilter(QDir::Files);
//    foreach(QString dirFile, dir.entryList())
//    {
//        dir.remove(dirFile);
//    }
    DocumentationViewer *newWingow = new DocumentationViewer(this);
    mDocumentationEngine->searchByKeyword(keyword);
    QString html = QString::fromStdString(HTMLContentGenerator::generate(mDocumentationEngine->documentationLinks()));
   // qDebug()<<html;
    if(mDocumentationEngine->documentationLinks().size() > 0)
    {
        qDebug()<<dir.path();
        QFile temp(QString(dir.path()+"/"+keyword+".html"));
        temp.open(QIODevice::WriteOnly);
        temp.write(html.toUtf8());
        temp.close();
       // newWingow->setHtml(html);
    }

    newWingow->webView()->load(QUrl::fromLocalFile(QString(dir.path()+"/"+keyword+".html")));
    mBrowseArea->addSubWindow(newWingow);
    newWingow->setAttribute(Qt::WA_DeleteOnClose);

}

void Browser::emptyDocumentationTab()
{


}

