#include "addcommenttextedit.h"
#include "ui_addcommenttextedit.h"

AddCommentTextEdit::AddCommentTextEdit(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddCommentTextEdit)
{
    ui->setupUi(this);
    ui->setBoldButton->setStyleSheet("font-weight: bold");
    ui->setItalicButton->setStyleSheet("font: italic");

    connect(ui->sendMessageButton, &QAbstractButton::clicked,              this, &AddCommentTextEdit::sendComment);
    connect(ui->setBoldButton,     &QAbstractButton::clicked,              this, &AddCommentTextEdit::setBoldPressed);
    connect(ui->setItalicButton,   &QAbstractButton::clicked,              this, &AddCommentTextEdit::setItalicPressed);
    connect(this,                  &AddCommentTextEdit::setBySpecialSigns, this, &AddCommentTextEdit::setSpecialSelect);
}

AddCommentTextEdit::~AddCommentTextEdit()
{
    delete ui;
}

QString AddCommentTextEdit::getText()
{
    return ui->commentTextEdit->toPlainText();
}

void AddCommentTextEdit::setText(const QString &text)
{
    ui->commentTextEdit->setPlainText(text);
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

    QTextCursor curs = ui->commentTextEdit->textCursor();
    QString after = ui->commentTextEdit->textCursor().selectedText();

    after.prepend(sighns);
    after.append(sighns);

    ui->commentTextEdit->textCursor().keepPositionOnInsert();
    QString currString = ui->commentTextEdit->toPlainText();
    currString.replace(
                selectionStart,
                selectionEnd - selectionStart,
                after);

    //ui->commentTextEdit->setText(currString);
    ui->commentTextEdit->setPlainText(currString);
    curs.setPosition(selectionStart + sighns.length());
    ui->commentTextEdit->setTextCursor(curs);
    ui->commentTextEdit->setFocus();
}

void AddCommentTextEdit::sendComment()
{
    if(ui->commentTextEdit->toPlainText().isEmpty())
    {
        emit emptyComment();
    }
    else
    {
        emit notEmptyComment();
    }
}
