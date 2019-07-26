#include "projectviewerdock.h"

#include <QDockWidget>
#include <QException>
#include <QTreeView>
#include <QWidget>
#include <QSettings>

#include "mainwindow.h"

ProjectViewerDock::ProjectViewerDock(QWidget *pParent): QDockWidget(pParent)
{
    setWindowTitle("Project Viewer");
    pTreeViewer = new QTreeView;
    setWidget(pTreeViewer);
    auto pMainWindow = dynamic_cast<MainWindow*>(pParent);

}
ProjectViewerDock::~ProjectViewerDock()
{
    QSettings settings("425", "PairStorm");
    settings.setValue("ProjectViewerDockGeometry", saveGeometry());
}
