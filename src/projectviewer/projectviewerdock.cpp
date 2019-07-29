#include "projectviewerdock.h"

#include <QDockWidget>
#include <QException>
#include <QSettings>
#include <QTreeView>
#include <QWidget>
#include <QDir>

#include "projectviewermodel.h"
#include "projecttreeview.h"
#include "mainwindow.h"

ProjectViewerDock::ProjectViewerDock(QWidget *pParent): QDockWidget(pParent)
{
    setWindowTitle("Project Viewer");

    QStringList filters;
    filters << "*.txt"<<"*.cpp"<<"*.h"<<"*.json"<<"*.c"<<"*.hpp";

    mpViewerModel = new ProjectViewerModel(this);
    mpTreeViewer = new ProjectTreeView(mpViewerModel,this);
    //for testing use own path
    //QDir dir = QDir::currentPath();

    setDir(QDir::current());
    setFilters(filters);

    setWidget(mpTreeViewer);

    auto pMainWindow = qobject_cast<MainWindow*>(pParent);

    if(!pMainWindow)
        return;

    connect(mpTreeViewer, &ProjectTreeView::codeFileSelected,
            pMainWindow, &MainWindow::onOpenFileFromProjectViewer);


}

void ProjectViewerDock::setFilters(QStringList filters)
{
    mpTreeViewer->setFilters(filters);
}

void ProjectViewerDock::setDir(QDir curDir)
{
    mpTreeViewer->setDirectory(curDir);
}

ProjectViewerDock::~ProjectViewerDock()
{
    QSettings settings("425", "PairStorm");
    settings.setValue("ProjectViewerDockGeometry", saveGeometry());
}
