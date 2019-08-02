#include "projectviewerdock.h"

#include <QItemSelectionModel>
#include <QDockWidget>
#include <QException>
#include <QTreeView>
#include <QWidget>
#include <QDebug>
#include <QDir>

#include "projectviewermodel.h"
#include "projecttreeview.h"
#include "mainwindow.h"

ProjectViewerDock::ProjectViewerDock(QWidget *pParent): QDockWidget(pParent)
{
    setWindowTitle("Project Viewer");

    auto pMainWindow = qobject_cast<MainWindow*>(pParent);

    if(!pMainWindow)
        return;

    // constructing filters
    QStringList filters = pMainWindow->getFileExtensions();
    for(auto& item: filters)
        item.push_front('*');

    mpViewerModel = new ProjectViewerModel(this);
    mpTreeViewer = new ProjectTreeView(mpViewerModel,this);
    mpTreeViewer->setCurrentIndex(mpViewerModel->index(0,0));

    setDir(QDir::current());
    setFilters(filters);

    setWidget(mpTreeViewer);    

    connect(mpTreeViewer, &ProjectTreeView::codeFileSelected,
            pMainWindow, &MainWindow::onOpenFileFromProjectViewer);
    setMinimumWidth(150);
}

void ProjectViewerDock::setFilters(QStringList filters)
{
    mpTreeViewer->setFilters(filters);
}

void ProjectViewerDock::setDir(QDir curDir)
{
    mpTreeViewer->setDirectory(curDir);
}

