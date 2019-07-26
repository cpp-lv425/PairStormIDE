#include "projecttreeview.h"

#include "projectviewermodel.h"
#include <QDebug>

ProjectTreeView::ProjectTreeView(ProjectViewerModel *model,QWidget *parent)
    : QTreeView(parent)
{
    setSelectionMode(QAbstractItemView::SingleSelection);
    setModel(model);
    this->mPrModel = model;
    foreach(const auto &temp, model->mFilesInfo)
    {
        qDebug()<<*temp;
    }
}

void ProjectTreeView::keyPressEvent(QKeyEvent *event)
{
    if(selectedIndexes().size() == 0)
    {
        qDebug()<<"no";
    }
    else
    {
        qDebug()<<mPrModel->fileInfo(selectedIndexes().at(0));
        QTreeView::keyPressEvent(event);
    }

}
/*void ProjectTreeView::mouseClickEvent(QMouseEvent *event)
{
    ProjectViewerModel *model = dynamic_cast<ProjectViewerModel *>(selectionModel());
   // if(model->isDir(selectedIndexes().first()))
   // {
    //QDir dir = model->
   // }
    //qDebug()<<*model->mFilesInfo[model->fileName(selectedIndexes().first())];

    //QTreeView::mo*///useDoubleClickEvent(event);
//}
