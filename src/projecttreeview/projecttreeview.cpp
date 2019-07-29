#include "projecttreeview.h"

#include "projectviewermodel.h"
#include <QKeyEvent>
#include <QDebug>

ProjectTreeView::ProjectTreeView()
{
    setSelectionMode(QAbstractItemView::SingleSelection);
    setIconSize(QSize(20,28));
    setVisible(true);
    hideColumn(1);
    hideColumn(2);
    hideColumn(3);
}

ProjectTreeView::ProjectTreeView(ProjectViewerModel *model,QWidget *parent)
    : QTreeView(parent)
{
    setSelectionMode(QAbstractItemView::SingleSelection);
    setModel(model);
    mProjectModel = model;
    setIconSize(QSize(20,28));
    setRootIndex(model->index(model->rootPath()));
    setVisible(true);
    hideColumn(1);
    hideColumn(2);
    hideColumn(3);
}

void ProjectTreeView::setProjectModel(ProjectViewerModel *model)
{
    setModel(model);
    mProjectModel = model;
    setRootIndex(model->index(model->rootPath()));
}

void ProjectTreeView::keyPressEvent(QKeyEvent *event)
{
    QFileInfo info = (mProjectModel->fileInfo(selectedIndexes().at(0)));
    if(event->key() == Qt::Key_Enter && selectedIndexes().size() != 0 && info.isFile())
    {
        qDebug()<<info.filePath();
        emit codeFileSelected(info.filePath());
    }
    QTreeView::keyPressEvent(event);
}

void ProjectTreeView::mouseDoubleClickEvent(QMouseEvent *event)
{
    QFileInfo info = (mProjectModel->fileInfo(selectedIndexes().at(0)));
    if(selectedIndexes().size() != 0 && info.isFile())
    {        
        emit codeFileSelected(info.filePath());
        qDebug()<<info.filePath();
    }
    QTreeView::mouseDoubleClickEvent(event);
}

void ProjectTreeView::setFilters(const QStringList &filters)
{
    mProjectModel->setFilters(filters);
}

void ProjectTreeView::setDirectory(const QDir &directory)
{
    mProjectModel->setDir(directory);
}
