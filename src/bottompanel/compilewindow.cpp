#include "compilewindow.h"
#include "ui_compilewindow.h"
#include "compiler/compilercontroler.h"
#include <QListWidgetItem>
#include <QGridLayout>

CompileWindow::CompileWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CompileWindow)
{
    ui->setupUi(this);
    mpCompileOutputList = new QListWidget;
    mpCompileOutputList->setLayoutMode(QListView::Batched);

    QGridLayout *compileOutputLayout = new QGridLayout(this);
    compileOutputLayout->addWidget(mpCompileOutputList);
    this->setLayout(compileOutputLayout);
}

CompileWindow::~CompileWindow()
{
    delete ui;
}

void CompileWindow::setCompileErrorsOutput(QString text)
{
    auto allErrorslist = getAllErrorsFromCompileOutput(text);
    for (auto error : allErrorslist)
    {
        auto *errorItem = new QListWidgetItem(error);
        if (error.contains(": warning: "))//if its warnings
        {
           errorItem->setForeground(Qt::yellow);
        }
        else
        {
           errorItem->setForeground(Qt::red);//if its error
        }
        mpCompileOutputList->addItem(errorItem);
    }
}

void CompileWindow::setCompileSuccessOutput()
{
    if (!mpCompileOutputList->count())
    {
        auto *errorItem = new QListWidgetItem("The build went without any errors and warnings!");
        mpCompileOutputList->addItem(errorItem);
    }
}

void CompileWindow::clearCompileOutputView()
{
    mpCompileOutputList->clear();
}

QStringList CompileWindow::getAllErrorsFromCompileOutput(const QString &compileErrorsOutput)
{
    QStringList rSeparatedErrorsList;
    auto errorsOutputLines = compileErrorsOutput.split('\n');

    QString separateError;
    for (auto errorOutputLine : errorsOutputLines)
    {
        auto line = removeAllSymbolsFromString(errorOutputLine, '~').simplified();
        line = removeAllSymbolsFromString(line, '|');
        if (line.contains('^'))
        {
            rSeparatedErrorsList.push_back(separateError);
            separateError.clear();
        }
        else
        {
            separateError += line + "\n";
        }
    }

    return rSeparatedErrorsList;
}

QString CompileWindow::removeAllSymbolsFromString(QString &outputLine, const char &symb)
{
    return outputLine.replace(symb, ' ');
}

