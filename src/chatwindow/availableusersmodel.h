#ifndef AVAILABLEUSERSMODEL_H
#define AVAILABLEUSERSMODEL_H

#include <QAbstractListModel>

class AvailableUsersModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit AvailableUsersModel(QObject *parent = nullptr);

    enum {
        NameRole = Qt::UserRole,
        ConnectedRole
    };

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;

    virtual QHash<int, QByteArray> roleNames() const override;

private:
};

#endif // AVAILABLEUSERSMODEL_H
