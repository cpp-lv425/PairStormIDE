#include "documentationviewer.h"

#include <QDir>
#include <QUrl>
#include <QListView>
#include <QWebEngineView>

#include "connectionmanager.h"

DocumentationViewer::DocumentationViewer(QWidget *parent) :
    QDialog(parent)
{
    mWebView = new QWebEngineView(this);
    mManager = new ConnectionManager(this);

    if(mManager->hasConnection())
    {
        mWebView->setUrl(QUrl("https://en.cppreference.com/w/"));
    }
    else
    {

    }

}

DocumentationViewer::~DocumentationViewer()
{
}
