#include "projectviewermodel.h"

#include <QImage>
#include <QApplication>

ProjectViewerModel::ProjectViewerModel(QObject *parent)
    : QFileSystemModel (parent)
{

}

ProjectViewerModel::ProjectViewerModel(const QDir &directory, const QStringList &filters, QObject *parent)
    : QFileSystemModel (parent)
{
    //dir = new QIcon("DIR2.png");
    //cpp = new QIcon("CPP2.png");
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

            QImage img;

           return QVariant();
           // return *dir;
        }
        else
        {
           // return *cpp;
        }
    }
    return QVariant();
}

