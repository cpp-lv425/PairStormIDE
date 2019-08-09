#include "addcommenttextedit.h"
#include "ui_addcommenttextedit.h"
#include<QFontDialog>
#include<QFont>

AddCommentTextEdit::AddCommentTextEdit(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddCommentTextEdit)
{
    ui->setupUi(this);
}

AddCommentTextEdit::~AddCommentTextEdit()
{
    delete ui;
}

void AddCommentTextEdit::on_setFont_clicked()
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok, this);
    if (ok)
    {
        ui->textEdit->setFont(font);
    }
    else
    {
        return;
    }

}
