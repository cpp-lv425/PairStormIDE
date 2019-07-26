#include "projecttreeview.h"

#include "projectviewermodel.h"
#include <QDebug>

ProjectTreeView::ProjectTreeView(QWidget *parent)
    : QTreeView(parent)
{
    setSelectionMode(QAbstractItemView::SingleSelection);
}

void ProjectTreeView::mouseDoubleClickEvent(QMouseEvent *event)
{
    ProjectViewerModel *model = dynamic_cast<ProjectViewerModel *>(selectionModel());
   // if(model->isDir(selectedIndexes().first()))
   // {
        qDebug()<<model->fileName(selectedIndexes().at(0));
   // }
    QTreeView::mouseDoubleClickEvent(event);
}
