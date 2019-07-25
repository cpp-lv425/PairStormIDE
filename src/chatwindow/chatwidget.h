#ifndef CHATWIDGET_H
#define CHATWIDGET_H

#include <QWidget>

class QPlainTextEdit;
class QLineEdit;

class ChatWidget: public QWidget
{
    Q_OBJECT

    QPlainTextEdit *pFeed;
    QLineEdit *pEnterLine;
public:
    ChatWidget(QWidget *pParent = nullptr);
};

#endif // CHATWIDGET_H
