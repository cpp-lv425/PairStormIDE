#include "addcommenttextedit.h"
#include "ui_addcommenttextedit.h"
#include <QDebug>

AddCommentTextEdit::AddCommentTextEdit(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddCommentTextEdit)
{
    ui->setupUi(this);
    ui->setBoldButton->setStyleSheet("font-weight: bold");
    ui->setItalicButton->setStyleSheet("font: italic");
}

AddCommentTextEdit::~AddCommentTextEdit()
{
    delete ui;
}

QString AddCommentTextEdit::getText()
{
    return ui->commentTextEdit->toPlainText();
}

QPushButton* AddCommentTextEdit::getSendButton()
{
    return ui->sendMessageButton;
}

void AddCommentTextEdit::setBoldSelect()
{

}
