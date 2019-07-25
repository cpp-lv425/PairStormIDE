#include "projecttreeview.h"

#include "projectviewermodel.h"

#include <QDebug>

void ProjectTreeView::mouseDoubleClickEvent(QMouseEvent *event)
{
    qDebug()<<"Double click";
    for(auto a : selectedIndexes())
    {
        qDebug()<<mModel->filePath(a);
    }
        QTreeView::mouseDoubleClickEvent(event);
    //view()->selectionModel()->selectedIndexes() to obtain selected indexes and fileSystemModel->filePath()

}
