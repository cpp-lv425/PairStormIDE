#ifndef ADDCOMMENTBUTTON_H
#define ADDCOMMENTBUTTON_H

#include<QObject>
#include<QPushButton>
#include<QWidget>
#include<QMouseEvent>
#include<QTextBlock>
#include<QPlainTextEdit>
#include<QtCore/QObject>


class AddCommentButton: public QPushButton
{
    Q_OBJECT

public:
    AddCommentButton(QWidget *parent = nullptr);
    ~AddCommentButton();
    void setCurrentLine(int currentLine);
    int getCurrentLine();

    QString getCommentString() const;
    void setCommentString(const QString &value);

signals:
    void addCommentButtonPressed(int);
    void mouseEnteredButtonArea(int);
    void mouseLeftButtonArea();

protected:
    virtual void mousePressEvent(QMouseEvent *event);

private:
    int mCurrentLine;
    QString commentString;
};

#endif // ADDCOMMENTBUTTON_H
