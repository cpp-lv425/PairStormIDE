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
    setIconSize(QSize(20,28));
    setRootIndex(model->index(model->rootPath()));
    setVisible(true);
    hideColumn(1);
    hideColumn(2);
    hideColumn(3);
}

void ProjectTreeView::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Enter && selectedIndexes().size() != 0)
    {
        emit codeFileSelected(mPrModel->fileInfo(selectedIndexes().at(0)).filePath());
    }
    QTreeView::keyPressEvent(event);
}

void ProjectTreeView::mouseDoubleClickEvent(QMouseEvent *event)
{
    if(selectedIndexes().size() != 0)
    {
        emit codeFileSelected(mPrModel->fileInfo(selectedIndexes().at(0)).filePath());
    }
    QTreeView::mouseDoubleClickEvent(event);
}
