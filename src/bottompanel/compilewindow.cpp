#include "compilewindow.h"
#include "ui_compilewindow.h"
#include "compiler/compilercontroler.h"
#include <QGridLayout>
#include <QDebug>
CompileWindow::CompileWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CompileWindow)
{
    ui->setupUi(this);
    compileEditor = new CodeEditor;
    QGridLayout *compileOutputLayout = new QGridLayout(this);
    compileOutputLayout->addWidget(compileEditor);
    this->setLayout(compileOutputLayout);
    //compileEditor->setReadOnly(true);
}

CompileWindow::~CompileWindow()
{
    delete ui;
}

void CompileWindow::setCompileOutput(QString text)
{
    qDebug()<<"set text to compiler output";
    compileEditor->setPlainText(text);
}

void CompileWindow::on_compileButton_clicked()
{
    emit programIsReadyToCompile();
}

