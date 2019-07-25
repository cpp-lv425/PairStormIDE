#include "projectviewermodel.h"

#include <QImage>
#include <QDebug>
#include <QFile>
#include <QApplication>

ProjectViewerModel::ProjectViewerModel(const QDir &directory, const QStringList &filters, QObject *parent)
    : QFileSystemModel (parent)
{
    setRootPath(directory.path());
    setFilter(QDir::AllDirs | QDir::Files | QDir::NoDotAndDotDot);
    setNameFilterDisables(false);
    setNameFilters(filters);

    QDirIterator it(":/img/", QDirIterator::FollowSymlinks);
    while (it.hasNext())
    {
        it.next();
        mImages.push_back(QPair<QString*,QIcon*>(new QString(it.fileInfo().baseName()),new QIcon(it.fileName())));
    }
    for(auto a : mImages)
    {
        qDebug()<<*a.first;
    }
}

QVariant ProjectViewerModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
    {
        return QVariant();
    }

    switch (role)
    {
    case Qt::DisplayRole:
    {
        QFileInfo info = fileInfo(index);
        return info.fileName();
    }
    case Qt::DecorationRole:
        if(isDir(index))
        {
            foreach(const auto & temp, mImages)
            {
                if(*temp.first == "dir")
                {
                    return *temp.second;
                }
            }
        }
        else
        {
            foreach(const auto & temp, mImages)
            {
                if(fileInfo(index).suffix() == *temp.first)
                {
                    return *temp.second;
                }
            }
        }
    }
    return QVariant();
}
