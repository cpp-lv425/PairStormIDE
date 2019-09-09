#include "compilewindow.h"
#include "ui_compilewindow.h"

CompileWindow::CompileWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CompileWindow)
{
    ui->setupUi(this);
}

CompileWindow::~CompileWindow()
{
    delete ui;
}

void CompileWindow::on_compileButton_clicked()
{
    emit programIsReadyToCompile();
}
