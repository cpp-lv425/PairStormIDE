#ifndef AVAILABLEUSERSMODEL_H
#define AVAILABLEUSERSMODEL_H

#include <QAbstractListModel>

class OnlineUsersList;

class OnlineUsersModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(OnlineUsersList * usersList READ getMpOnlineUsersList WRITE setMpOnlineUsersList)

public:
    explicit OnlineUsersModel(QObject *parent = nullptr);

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

    OnlineUsersList *getMpOnlineUsersList() const;
    void setMpOnlineUsersList(OnlineUsersList *value);

private:

    OnlineUsersList * mpOnlineUsersList;
};

#endif // AVAILABLEUSERSMODEL_H
