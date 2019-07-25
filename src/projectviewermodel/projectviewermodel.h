#ifndef PROJECTVIEWERMODEL_H
#define PROJECTVIEWERMODEL_H

#include <memory>

#include <QPair>
#include <QVector>
#include <QFileSystemModel>

QT_BEGIN_NAMESPACE
class QStringList;
class QIcon;
QT_END_NAMESPACE

class ProjectViewerModel : public QFileSystemModel
{
public:
    ProjectViewerModel(const QDir &directory,const QStringList &filters, QObject *parent = nullptr);
    void setFilters(const QStringList &list);

    // QAbstractItemModel interface
public:
    QVariant data(const QModelIndex &index, int role) const;

private:
    QVector<QPair<QString*,QIcon*>> mImages;
};

#endif // PROJECTVIEWERMODEL_H
