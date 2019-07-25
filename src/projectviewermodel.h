#ifndef PROJECTVIEWERMODEL_H
#define PROJECTVIEWERMODEL_H

#include <QFileSystemModel>
#include <QIcon>
QT_BEGIN_NAMESPACE
class QStringList;
QT_END_NAMESPACE

class ProjectViewerModel : public QFileSystemModel
{
public:
    ProjectViewerModel(QObject *parent);
    ProjectViewerModel(const QDir &directory,const QStringList &filters, QObject *parent = nullptr);
    void setFilters(const QStringList &list);

    // QAbstractItemModel interface
public:
    QVariant data(const QModelIndex &index, int role) const;
    QIcon *dir;
    QIcon *cpp;
};

#endif // PROJECTVIEWERMODEL_H
