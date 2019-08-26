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

    explicit ChatWidgetInterface(QWidget *qObject = nullptr) : QWidget(qObject) {}

    // Possible chat themes & current theme variable
    enum Theme {
        DefaultTheme,
        WhiteTheme,
        BlueTheme,
        DarkTheme
    };

    // Theme name -> theme dictionary
    QHash<QString, Theme> mThemes
    {
        { "WHITE", Theme::WhiteTheme },
        { "BLUE",  Theme::BlueTheme  },
        { "DARK",  Theme::DarkTheme  }
    };

public:

    //======================= Docker key events processor ====================
    virtual void keyPressEvent(QKeyEvent * event) = 0;

public slots:

    //======================= Slots for GUI events ===========================
    virtual void configureOnLogin(const QString & userName)  = 0;
    virtual void updateTheme     (const QString & themeName) = 0;

    //======================= Slots for Network events ========================
    virtual void updateOnlineUsers   (const QStringList & onlineUserNames)    = 0;
    virtual void updateConnectedUsers(const QStringList & connectedUserNames) = 0;

    virtual void appendMessage(const QString & messageAuthor,
                               const QString & messageBogy) = 0;

signals:

    //======================= Signals to Network =============================
    void startSharingRequested(const QString & userName);
    void stopSharingRequested (const QString & userName);

    void messageSent(const QString & messageBody);
};

#endif // CHATWIDGETINTERFACE_H
