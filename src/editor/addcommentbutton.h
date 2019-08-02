#ifndef ADDCOMMENTBUTTON_H
#define ADDCOMMENTBUTTON_H

#include<QObject>
#include<QPushButton>
#include<QWidget>
#include<QMouseEvent>
#include<QTextBlock>
#include<QPlainTextEdit>
#include <QtCore/QObject>

class AddCommentButton: public QPushButton
{
    Q_OBJECT

public:
    AddCommentButton(QWidget *parent = nullptr);
    ~AddCommentButton();
    void setCurrentLine(int currentLine);
    int getCurrentLine();

signals:
    void addCommentButtonPressed(int);

protected:
    virtual void mousePressEvent(QMouseEvent *event);

private:
    int mCurrentLine;
};

#endif // ADDCOMMENTBUTTON_H
