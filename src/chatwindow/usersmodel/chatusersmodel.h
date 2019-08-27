#ifndef CHATUSERSMODEL_H
#define CHATUSERSMODEL_H

#include <QAbstractListModel>
#include "chatuserscontroller.h"

// ==========================================================================================
//                                                                                USERS MODEL
// ==========================================================================================
class ChatUsersModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(ChatUsersController *list READ list WRITE setList)

public:

    explicit ChatUsersModel(QObject *parent = nullptr);

    // Users getters for the qml
    int rowCount(const QModelIndex &modelIndex = QModelIndex()) const override;
    QVariant data(const QModelIndex &modelIndex,
                  int role = Qt::DisplayRole) const override;

    // Users' attributes setter for the qml
    bool setData(const QModelIndex &modelIndex,
                 const QVariant &newValue,
                 int role = Qt::EditRole) override;

    // Attribute names getter
    virtual QHash<int, QByteArray> roleNames() const override;

    // Local messages controller getter & setter
    ChatUsersController* list() const;
    void setList(ChatUsersController *newController);

private:

    // Ids of the user' attributes
    enum {
        UserNameRole = Qt::UserRole,
        UserConnectedRole,
        UserOnlineRole
    };
    // Local users controller used by model
    ChatUsersController *mpUsersController;
};

#endif // CHATUSERSMODEL_H
