#include "addcommentbutton.h"

AddCommentButton::AddCommentButton(QWidget *parent):QPushButton (parent)
{}

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


