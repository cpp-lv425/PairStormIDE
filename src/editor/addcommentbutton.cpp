#include "addcommentbutton.h"

AddCommentButton::AddCommentButton(QWidget *parent):QPushButton (parent)
{
}

AddCommentButton::~AddCommentButton() = default;

void AddCommentButton::setCurrentLine(int currentLine)
{
    mCurrentLine = currentLine;
}

int AddCommentButton::getCurrentLine()
{
    return mCurrentLine;
}

void AddCommentButton::mousePressEvent(QMouseEvent *event)
{
    QPushButton::mousePressEvent(event);
    emit addCommentButtonPressed(mCurrentLine);
}

QString AddCommentButton::getUser() const
{
    return mUser;
}

void AddCommentButton::setUser(const QString &user)
{
    mUser = user;
}

QString AddCommentButton::getCommentString() const
{
    return mCommentString;
}

void AddCommentButton::setCommentString(const QString &value)
{
    mCommentString = value;
}
