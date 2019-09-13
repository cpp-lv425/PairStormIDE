#include "compilewindow.h"
#include "ui_compilewindow.h"
#include "compiler/compilercontroler.h"
#include <QGridLayout>
#include <QDebug>
#include <QListWidgetItem>

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
    qDebug() << "try to set compilation output";
    qDebug() << "   errors: " << text;
    compileOutputList->clear();
    auto allErrorslist = getAllErrorsFromCompileOutput(text);
    for (auto error : allErrorslist)
    {
        qDebug() << "      error is: " << error;
        auto *errorItem = new QListWidgetItem(error);
        if (error.contains(": warning: "))//if its warnings
        {
           errorItem->setForeground(Qt::yellow);
        }
        else
        {
           errorItem->setForeground(Qt::red);//if its error
        }
        compileOutputList->addItem(errorItem);
    }
}

void CompileWindow::clearCompileOutputView()
{
    compileOutputList->clear();
}

QStringList CompileWindow::getAllErrorsFromCompileOutput(const QString &compileErrorsOutput)
{
    QStringList rSeparatedErrorsList;
    auto errorsOutputLines = compileErrorsOutput.split('\n');

    qDebug() << "errors inside parsing";
    std::for_each(errorsOutputLines.cbegin(), errorsOutputLines.cend(), [](const QString & elem) {qDebug() << "   " << elem << ", ";});

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

    qDebug() << "errors inside parsing";
    std::for_each(rSeparatedErrorsList.cbegin(), rSeparatedErrorsList.cend(), [](const QString & elem) {qDebug() << "   " << elem << ", ";});

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
