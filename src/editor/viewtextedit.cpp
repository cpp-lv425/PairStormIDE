#include "viewtextedit.h"
#include "ui_viewtextedit.h"

ViewTextEdit::ViewTextEdit(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ViewTextEdit)
{
    ui->setupUi(this);
    ui->viewTextEdit->setReadOnly(true);
    ui->viewTextEdit->setLineWrapMode(QPlainTextEdit::NoWrap);
}

ViewTextEdit::~ViewTextEdit()
{
    delete ui;
}

QString ViewTextEdit::getText()
{
    return ui->viewTextEdit->toPlainText();
}

void ViewTextEdit::setText(const QString &str)
{
    ui->viewTextEdit->setPlainText(str);
}

QTextDocument* ViewTextEdit::getDocument()
{
    return ui->viewTextEdit->document();
}
