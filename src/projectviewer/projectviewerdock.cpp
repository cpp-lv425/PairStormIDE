#include "projectviewerdock.h"

#include <QItemSelectionModel>
#include <QDockWidget>
#include <QException>
#include <QTreeView>
#include <QWidget>
#include <QDir>

#include "projectviewermodel.h"
#include "projecttreeview.h"
#include "mainwindow.h"

ProjectViewerDock::ProjectViewerDock(QStringList filters,
                                     QWidget *pParent):
    QDockWidget(pParent)
{
    setWindowTitle("Project Viewer");

    // constructing filters
    for (auto &item: filters)
    {
        item.push_front('*');
    }

    // creating an instance of model
    mpViewerModel = new ProjectViewerModel(this);
    // creating an instance of view
    mpTreeViewer = new ProjectTreeView(mpViewerModel,this);
    mpTreeViewer->setCurrentIndex(mpViewerModel->index(0,0));

    setDir(QDir::current());
    setFilters(filters);

    setWidget(mpTreeViewer);    

    mpTreeViewer->setMinimumWidth(pParent->width() / 3);
    connect(mpTreeViewer, &ProjectTreeView::codeFileSelected,
            this, &ProjectViewerDock::onOpenFileFromProjectViewer);    
}

void ProjectViewerDock::setFilters(QStringList filters)
{
    mpTreeViewer->setFilters(filters);
}

void ProjectViewerDock::setDir(QDir curDir)
{
    mpTreeViewer->setDirectory(curDir);
}

void ProjectViewerDock::onOpenFileFromProjectViewer(QString fileName)
{
    emit openFileFromProjectViewer(fileName);
}
