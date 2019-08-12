#include "documentmanager.h"

#include <QSplitter>
#include <QMdiArea>
#include <QVector>

#include <textdocumentholder.h>

DocumentManager::DocumentManager()
{
    mpSplitter = new QSplitter;
    splitWindow();
    splitWindow();
    splitWindow();

}

void DocumentManager::splitWindow()
{
    QMdiArea *pNewArea = createMdiArea();

    mpSplitter->addWidget(pNewArea);
    mDocAreas.push_back(pNewArea);

    TextDocumentHolder *doc = new TextDocumentHolder;
    pNewArea->addSubWindow(doc);
    doc->setWindowState(Qt::WindowMaximized);
}

QSplitter* DocumentManager::getSplitter()
{
    return mpSplitter;
}

QMdiArea* DocumentManager::createMdiArea()
{
    QMdiArea *pMdiArea = new QMdiArea;
    pMdiArea->setTabsClosable(true);
    pMdiArea->setViewMode(QMdiArea::TabbedView);
    return pMdiArea;
}
