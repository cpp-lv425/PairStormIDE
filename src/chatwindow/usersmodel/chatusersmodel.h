#ifndef CHATUSERSMODEL_H
#define CHATUSERSMODEL_H

#include <QAbstractListModel>
#include "chatuserscontroller.h"

class ChatUsersModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(ChatUsersController *list READ list WRITE setList)

public:
    explicit ChatUsersModel(QObject *parent = nullptr);

    enum {
        UserNameRole = Qt::UserRole,
        UserConnectedRole,
        UserOnlineRole
    };


    int rowCount(const QModelIndex &modelIndex = QModelIndex()) const override;
    QVariant data(const QModelIndex &modelIndex,
                  int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index,
                 const QVariant &value,
                 int role = Qt::EditRole) override;

    virtual QHash<int, QByteArray> roleNames() const override;

    ChatUsersController * list() const;
    void setList(ChatUsersController * newList);

private:

    ChatUsersController * mpUsersController;
};

#endif // CHATUSERSMODEL_H
