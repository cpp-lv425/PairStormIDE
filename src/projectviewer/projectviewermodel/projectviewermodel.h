#ifndef PROJECTVIEWERMODEL_H
#define PROJECTVIEWERMODEL_H

#include <memory>
#include <string>

#include <QMap>
#include <QFileSystemModel>

QT_BEGIN_NAMESPACE
class QStringList;
class QIcon;
QT_END_NAMESPACE

class ProjectViewerModel : public QFileSystemModel
{
    Q_OBJECT
public:
    ProjectViewerModel(QObject *parent = nullptr);
    ProjectViewerModel(const QDir &directory,const QStringList &filters, QObject *parent = nullptr);
    ~ProjectViewerModel();
    void setFilters(const QStringList &list);
    void setDir(const QDir &directory);

    // QAbstractItemModel interface
public:
    QVariant data(const QModelIndex &index, int role) const;

private:
    QMap<std::string, QIcon *> mImages;
};

#endif // PROJECTVIEWERMODEL_H
