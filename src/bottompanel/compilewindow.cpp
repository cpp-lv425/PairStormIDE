#include "compilewindow.h"
#include "ui_compilewindow.h"
#include "compiler/compilercontroler.h"
#include <QGridLayout>
#include <QListWidgetItem>
#include <QDebug>

CompileWindow::CompileWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CompileWindow)
{
    ui->setupUi(this);
    compileOutputList = new QListWidget;
    compileOutputList->setLayoutMode(QListView::Batched);

    QGridLayout *compileOutputLayout = new QGridLayout(this);
    compileOutputLayout->addWidget(compileOutputList);
    this->setLayout(compileOutputLayout);
}

CompileWindow::~CompileWindow()
{
    delete ui;
}

void CompileWindow::setCompileOutput(QString text)
{
    qDebug()<<"setCompilerOut";
    compileOutputList->clear();
    auto allErrorslist = getAllErrorsFromCompileOutput(text);
    for (auto error : allErrorslist)
    {
        qDebug()<<"out = "<<error;
        auto *errorItem = new QListWidgetItem(error);
        if (error.contains(": warning: "))//if its warnings
        {
           errorItem->setForeground(Qt::yellow);
        }
        else if (error.contains(": error: "))
        {
           errorItem->setForeground(Qt::red);//if its error
        }
        compileOutputList->addItem(errorItem);
    }
}

QStringList CompileWindow::getAllErrorsFromCompileOutput(const QString &compileErrorsOutput)
{
    QStringList rSeparatedErrorsList;
    if (compileErrorsOutput == "No errors. Program executed with code 0.")
    {
        return rSeparatedErrorsList << compileErrorsOutput;
    }
    auto errorsOutputLines = compileErrorsOutput.split('\n');

    QString separateError;
    for (auto errorOutputLine : errorsOutputLines)
    {
        auto line = removeAllSymbolsFromString(errorOutputLine, '~').simplified();
        if (lineContainsOnlyOneSymbol(line, '^'))
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


bool CompileWindow::lineContainsOnlyOneSymbol(const QString &string, const char &symb) noexcept
{
    return string == static_cast<QString>(symb);
}
