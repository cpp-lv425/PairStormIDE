#include "compilewindow.h"
#include "ui_compilewindow.h"
#include "compiler/compilercontroler.h"
#include <QGridLayout>

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
    compileEditor->setPlainText(text);
}

void CompileWindow::on_compileButton_clicked()
{
    emit programIsReadyToCompile();
}

/*    consoleServiceProvider = new ConsoleServiceProvider;
    consoleView = new ConsoleView;

    QGridLayout *consoleViewGridLayout = new QGridLayout (this);
    consoleViewGridLayout->addWidget(consoleView);
    this->setLayout(consoleViewGridLayout);
*/
