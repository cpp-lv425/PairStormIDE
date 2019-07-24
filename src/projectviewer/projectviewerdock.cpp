#include "projectviewerdock.h"

#include <QDockWidget>
#include <QException>
#include <QTreeView>
#include <QWidget>

#include "mainwindow.h"

ProjectViewerDock::ProjectViewerDock(QWidget *pParent): QDockWidget(pParent)
{
    setWindowTitle("Project Viewer");
    pTreeViewer = new QTreeView;
    setWidget(pTreeViewer);
    auto pMainWindow = dynamic_cast<MainWindow*>(pParent);

    if(!pMainWindow)
        throw QException();

    pMainWindow->addDockWidget(Qt::LeftDockWidgetArea, this);
}
