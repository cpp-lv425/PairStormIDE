#include "projectviewerdock.h"

#include <QDockWidget>
#include <QException>
#include <QTreeView>
#include <QWidget>
#include <QSettings>
#include <QDir>
#include <thread>
#include <chrono>

#include "projectviewermodel.h"
#include "projecttreeview.h"
#include "mainwindow.h"

ProjectViewerDock::ProjectViewerDock(QWidget *pParent): QDockWidget(pParent)
{
    setWindowTitle("Project Viewer");
    QStringList filters;
    filters << "*.txt"<<"*.cpp"<<"*.h"<<"*.json"<<"*.c"<<"*.hpp";

    //for testing use own path
    QDir dir = QDir::currentPath();
    ProjectViewerModel* fileSystemModel = new ProjectViewerModel(dir,filters);
    pTreeViewer = new ProjectTreeView(fileSystemModel);
    setWidget(pTreeViewer);

    auto pMainWindow = qobject_cast<MainWindow*>(pParent);

    if(!pMainWindow)
        return;

    connect(pTreeViewer, &ProjectTreeView::codeFileSelected,
            pMainWindow, &MainWindow::onOpenFileFromProjectViewer);
}
ProjectViewerDock::~ProjectViewerDock()
{
    QSettings settings("425", "PairStorm");
    settings.setValue("ProjectViewerDockGeometry", saveGeometry());
}
