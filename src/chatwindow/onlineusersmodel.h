#ifndef AVAILABLEUSERSMODEL_H
#define AVAILABLEUSERSMODEL_H

#include <QAbstractListModel>
#include "onlineuserslist.h"

class OnlineUsersModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit OnlineUsersModel(QObject *parent = nullptr);

    enum Role {
        NameRole = Qt::UserRole,
        ConnectedRole
    };

    //======================= To get data from QML ===========================
    virtual QHash<int, QByteArray> roleNames() const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index,
                  int role = Qt::DisplayRole) const override;

    //======================= To set data from QML ===========================
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;


    Qt::ItemFlags flags(const QModelIndex& index) const override;

    //======================= To set data from backend =======================
    void updateOnlineUsers(const QStringList & userNames);
    void updateConnectedUsers(const QStringList & userNames);

signals:

    void stateChangedOn (const QString userName);
    void stateChangedOff(const QString userName);

private:

    OnlineUsersList * mpUsersList;

};

#endif // AVAILABLEUSERSMODEL_H
