#include "compilewindow.h"
#include "ui_compilewindow.h"
#include "compiler/compilercontroler.h"

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

void CompileWindow::on_Removeall_clicked()
{
    removeAllExecutableAndObjectsFiles();
}
