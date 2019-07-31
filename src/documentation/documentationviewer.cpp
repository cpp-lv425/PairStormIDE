#include "documentationviewer.h"

#include <QDir>
#include <QUrl>
#include <QListView>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QWebEngineView>

#include "connectionmanager.h"
#include "documentationengine.h"

DocumentationViewer::DocumentationViewer(QWidget *parent) :
    QDialog(parent)
{
    mWebView = new QWebEngineView(this);
    mManager = new ConnectionManager(this);
    mDocumentationEngine = new DocumentationWebEngine(this);
    mWebView->setUrl(QUrl("https://doc.qt.io/qt-5/qwebenginehistory.html"));

    QHBoxLayout *lHBoxLayout = new QHBoxLayout;
    lHBoxLayout->addWidget(mWebView);
    QVBoxLayout *lVBoxLayout = new QVBoxLayout;
    lVBoxLayout->addLayout(lHBoxLayout);
    setLayout(lVBoxLayout);

    setGeometry(geometry().x(), geometry().y(), 500, 300);
}
