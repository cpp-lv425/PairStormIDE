#include "browser.h"

#include "documentationviewer.h"

Browser::Browser(QWidget *parent) : QMainWindow (parent)
{
    mBrowseArea = new MDIArea(this);
    setCentralWidget(mBrowseArea);
}

void Browser::newTab(const QString &keyword)
{
    DocumentationViewer *neww1 = new DocumentationViewer;
    mBrowseArea->addSubWindow(neww1);
    neww1->setAttribute(Qt::WA_DeleteOnClose);
}

