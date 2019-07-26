#include "projecttreeview.h"

#include "projectviewermodel.h"
#include <QKeyEvent>
#include <QDebug>

ProjectTreeView::ProjectTreeView(ProjectViewerModel *model,QWidget *parent)
    : QTreeView(parent)
{
    setSelectionMode(QAbstractItemView::SingleSelection);
    setModel(model);
    mPrModel = model;
}

void ProjectTreeView::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Enter && selectedIndexes().size() != 0)
    {
        QString filename = mPrModel->fileInfo(selectedIndexes().at(0)).path() + mPrModel->fileInfo(selectedIndexes().at(0)).fileName();
        emit codeFileSelected(filename);
        qDebug()<<filename;
    }
    QTreeView::keyPressEvent(event);
}

void ProjectTreeView::mouseDoubleClickEvent(QMouseEvent *event)
{
    if(selectedIndexes().size() != 0)
    {
        QString filename = mPrModel->fileInfo(selectedIndexes().at(0)).path() + mPrModel->fileInfo(selectedIndexes().at(0)).fileName();
        emit codeFileSelected(filename);
        qDebug()<<filename;
    }
    QTreeView::mouseDoubleClickEvent(event);
}
