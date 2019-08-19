#include "projectviewermodel.h"

#include <QDebug>
#include <QImage>
#include <QStringList>

ProjectViewerModel::ProjectViewerModel(QObject *parent)
    : QFileSystemModel (parent)
{
    setFilter(QDir::AllDirs | QDir::Files | QDir::NoDotAndDotDot);
    setNameFilterDisables(false);

    QDirIterator it(":/img/", QDirIterator::Subdirectories);
    while (it.hasNext())
    {
        it.next();
        mImages.insert(it.fileInfo().baseName().toStdString(),new QIcon(it.path()+it.fileName()));
    }
}

ProjectViewerModel::ProjectViewerModel(const QDir &directory, const QStringList &filters, QObject *parent)
    : QFileSystemModel (parent)
{
    setRootPath(directory.path());
    setFilter(QDir::AllDirs | QDir::Files | QDir::NoDotAndDotDot);
    setNameFilterDisables(false);
    setNameFilters(filters);

    QDirIterator it(":/img/", QDirIterator::Subdirectories);
    while (it.hasNext())
    {
        it.next();
        mImages.insert(it.fileInfo().baseName().toStdString(),new QIcon(it.path()+it.fileName()));
    }
}

ProjectViewerModel::~ProjectViewerModel()
{
    foreach(auto &temp,mImages)
    {
        delete temp;
    }
}

void ProjectViewerModel::setFilters(const QStringList &filters)
{
    setNameFilters(filters);
}

void ProjectViewerModel::setDir(const QDir &directory)
{
    setRootPath(directory.path());
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
    case Qt::DecorationRole:
        if(isDir(index))
        {
            return *mImages["dir"];
        }
        else
        {
            return *mImages[fileInfo(index).suffix().toStdString()];
        }
    default:
         return  QFileSystemModel::data(index,role);
    }
}
