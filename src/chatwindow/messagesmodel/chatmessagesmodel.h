#ifndef CHATMESSAGESMODEL_H
#define CHATMESSAGESMODEL_H

#include <QAbstractListModel>
#include "chatmessagescontroller.h"

// ==========================================================================================
//                                                                             MESSAGES MODEL
// ==========================================================================================
class ChatMessagesModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(ChatMessagesController *list READ list WRITE setList)

public:

    explicit ChatMessagesModel(QObject *parent = nullptr);

    // Messages getters for the qml
    int rowCount(const QModelIndex &modelIndex = QModelIndex()) const override;
    QVariant data(const QModelIndex &modelIndex,
                  int role = Qt::DisplayRole) const override;

    // Attribute names getter
    virtual QHash<int, QByteArray> roleNames() const override;

    // Local messages controller getter & setter
    ChatMessagesController * list() const;
    void setList(ChatMessagesController * newList);

private:

    // Ids of the message' attributes
    enum {
        AuthorNameRole = Qt::UserRole,
        PublicationDateTimeRole,
        ContentRole,
        TypeRole
    };
    // Local messages controller used by model
    ChatMessagesController * mpMessagesController;
};

#endif // CHATMESSAGESMODEL_H
