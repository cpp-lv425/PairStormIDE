#include "commentwidget.h"
#include "ui_commentwidget.h"

CommentWidget::CommentWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CommentWidget)
{
    ui->setupUi(this);
}

CommentWidget::~CommentWidget()
{
    delete ui;
}
