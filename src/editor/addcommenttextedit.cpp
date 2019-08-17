#include "addcommenttextedit.h"
#include "ui_addcommenttextedit.h"

AddCommentTextEdit::AddCommentTextEdit(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddCommentTextEdit)
{
    ui->setupUi(this);
    ui->setBoldButton->setStyleSheet("font-weight: bold");
    ui->setItalicButton->setStyleSheet("font: italic");
    ui->commentTextEdit->setLineWrapMode(QPlainTextEdit::NoWrap);

    connect(ui->sendMessageButton,   &QAbstractButton::clicked,              this, &AddCommentTextEdit::sendComment);
    connect(ui->deleteMessageButton, &QAbstractButton::clicked,              this, &AddCommentTextEdit::deleteComment);
    connect(ui->setBoldButton,       &QAbstractButton::clicked,              this, &AddCommentTextEdit::setBoldPressed);
    connect(ui->setItalicButton,     &QAbstractButton::clicked,              this, &AddCommentTextEdit::setItalicPressed);
    connect(this,                    &AddCommentTextEdit::setBySpecialSigns, this, &AddCommentTextEdit::setSpecialSelect);
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

QPushButton *AddCommentTextEdit::getSetBoldButton()
{
    return ui->setBoldButton;
}

QPushButton *AddCommentTextEdit::getSetItalicButton()
{
    return ui->setItalicButton;
}

QTextCursor AddCommentTextEdit::getCursor()
{
    return ui->commentTextEdit->textCursor();
}

void AddCommentTextEdit::setCursor(const QTextCursor &cursor)
{
    ui->commentTextEdit->setTextCursor(cursor);
}

void AddCommentTextEdit::setBoldPressed()
{
    emit setBySpecialSigns("**");
}

void AddCommentTextEdit::setItalicPressed()
{
    emit setBySpecialSigns("_");
}

void AddCommentTextEdit::setSpecialSelect(QString sighns)//sights - symbols which we add to the start and to the end ("**", "_")
{
    int selectionStart = ui->commentTextEdit->textCursor().selectionStart();
    int selectionEnd   = ui->commentTextEdit->textCursor().selectionEnd();

    QTextCursor curs = ui->commentTextEdit->textCursor();
    QString replacingPartStr = ui->commentTextEdit->textCursor().selectedText();//the part of string which we select

    replacingPartStr.prepend(sighns);//add sight(sights) to the begin of selected string
    replacingPartStr.append(sighns);//add sight(sights) to the end of selected string
    //after this adding we've gotten new format text. For example after selectin word "code" as bold we'll get "**code**"

   // ui->commentTextEdit->textCursor().keepPositionOnInsert();
    QString ReplacedString = ui->commentTextEdit->toPlainText();//replace previous word (without sights) to the new one
    ReplacedString.replace(
                selectionStart,
                selectionEnd - selectionStart,
                replacingPartStr);

    ui->commentTextEdit->setPlainText(ReplacedString);//set this replaced text to the PlaintTextEdit

    //in the code belove we switch "current view window" again to the input text widget
    curs.setPosition(selectionStart + sighns.length());
    ui->commentTextEdit->setTextCursor(curs);
    ui->commentTextEdit->setFocus();
}

void AddCommentTextEdit::sendComment()
{
    if (ui->commentTextEdit->toPlainText().isEmpty())
    {
        emit emptyCommentWasSent();
    }
    else
    {
        emit notEmptyCommentWasSent();
    }
}

void AddCommentTextEdit::deleteComment()
{
    emit commentWasDeleted();
}
