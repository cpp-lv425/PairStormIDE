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

    connect(ui->setBoldButton,   SIGNAL(clicked()),                  this, SLOT(setBoldPressed()));
    connect(ui->setItalicButton, SIGNAL(clicked()),                  this, SLOT(setItalicPressed()));
    connect(this,                SIGNAL(setBySpecialSigns(QString)), this, SLOT(setSpecialSelect(QString)));
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

void AddCommentTextEdit::setBoldPressed()
{
    emit setBySpecialSigns("**");
}

void AddCommentTextEdit::setItalicPressed()
{
    emit setBySpecialSigns("_");
}

void AddCommentTextEdit::setSpecialSelect(QString sighns)
{
    int selectionStart = ui->commentTextEdit->textCursor().selectionStart();
    int selectionEnd = ui->commentTextEdit->textCursor().selectionEnd();
    auto selectedText = ui->commentTextEdit->textCursor().selectedText();

    QString after = selectedText.prepend(sighns);
    after.append(sighns);
    QString currString = ui->commentTextEdit->toPlainText();
    currString.replace(
                selectionStart,
                selectionEnd - selectionStart,
                after);

    ui->commentTextEdit->setText(currString);
    QTextCursor curs = ui->commentTextEdit->textCursor();
    qDebug()<<"curs pos = "<<curs.position();
    curs.setPosition(selectionStart);
   // ui->commentTextEdit->setCur
    ui->commentTextEdit->textCursor().setPosition(selectionStart);
}
