#include "projectviewermodel.h"

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
