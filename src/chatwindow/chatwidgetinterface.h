#ifndef CHATWIDGETINTERFACE_H
#define CHATWIDGETINTERFACE_H

#include <QWidget>

// ==========================================================================================
//                                                                             CHAT INTERFACE
// ==========================================================================================
class ChatWidgetInterface : public QWidget
{
    Q_OBJECT

protected:

    explicit ChatWidgetInterface(QWidget *qObject = nullptr) : QWidget(qObject)
    {
        mThemes["WHITE"] = Theme::WhiteTheme;
        mThemes["BLUE"]  = Theme::BlueTheme;
        mThemes["DARK"]  = Theme::DarkTheme;
    }

    enum Theme {
        DefaultTheme,
        WhiteTheme,
        BlueTheme,
        DarkTheme
    } mCurrentTheme = Theme::DefaultTheme;
    QHash<QString, Theme> mThemes;

public:

    virtual void keyPressEvent(QKeyEvent * event) = 0;

public slots:

    //======================= Slots for Network events =======================
    virtual void configureOnLogin(const QString & userName) = 0;

    virtual void updateOnlineUsers(const QStringList & onlineUserNames) = 0;
    virtual void updateConnectedUsers(const QStringList & connectedUserNames) = 0;

    virtual void appendMessage(const QString & messageAuthor,
                               const QString & messageBogy) = 0;

    virtual void updateTheme(const QString & themeName) = 0;

signals:

    //======================= Signals to Network =============================
    void startSharingRequested(const QString & userName);
    void stopSharingRequested(const QString & userName);

    void messageSent(const QString & messageBody);
};

#endif // CHATWIDGETINTERFACE_H
