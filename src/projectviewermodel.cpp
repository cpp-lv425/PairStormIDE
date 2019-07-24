#include "projectviewermodel.h"

#include <QIcon>
#include <QApplication>

ProjectViewerModel::ProjectViewerModel(QObject *parent)
    : QFileSystemModel (parent)
{

}

ProjectViewerModel::ProjectViewerModel(const QDir &directory, const QStringList &filters, QObject *parent)
    : QFileSystemModel (parent)
{
    setRootPath(directory.path());
    setFilter(QDir::AllDirs | QDir::Files | QDir::NoDotAndDotDot);
    setNameFilterDisables(false);
    setNameFilters(filters);


}

QVariant ProjectViewerModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
    {
        return QVariant();
    }

    QFileInfo info = fileInfo(index);
    switch (role)
    {
    case Qt::DisplayRole:
        return info.fileName();
    case Qt::DecorationRole:
        if(isDir(index))
        {
            QIcon icon("dir.png");
            return icon;
        }
        else
        {
            QIcon icon("icon.png");
            return icon;
        }
    }
    return QVariant();
}

