#ifndef CHATMESSAGESMODEL_H
#define CHATMESSAGESMODEL_H

#include <QAbstractListModel>
#include "chatmessagescontroller.h"

class ChatMessagesModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(ChatMessagesController *list READ list WRITE setList)

public:
    explicit ChatMessagesModel(QObject *parent = nullptr);

    enum {
        AuthorNameRole = Qt::UserRole,
        PublicationDateTimeRole,
        ContentRole,
        TypeRole
    };


    int rowCount(const QModelIndex &modelIndex = QModelIndex()) const override;
    QVariant data(const QModelIndex &modelIndex,
                  int role = Qt::DisplayRole) const override;


    virtual QHash<int, QByteArray> roleNames() const override;

    ChatMessagesController * list() const;
    void setList(ChatMessagesController * newList);

private:

    ChatMessagesController * mpMessagesController;
};

#endif // CHATMESSAGESMODEL_H
