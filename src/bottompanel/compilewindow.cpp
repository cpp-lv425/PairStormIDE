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
    compileOutputList->clear();
    auto list = getAllErrorsFromCompileOutput(text);
    for (auto error : list)
    {
        QListWidgetItem *itm = new QListWidgetItem(error);
        if (error.contains(": warning: "))
        {
           itm->setForeground(Qt::yellow);
        }
        else
        {
           itm->setForeground(Qt::red);
        }
        compileOutputList->addItem(itm);
    }
}

QStringList CompileWindow::getAllErrorsFromCompileOutput(const QString &compileErrorsOutput)
{
    QStringList errorsOutputLines = compileErrorsOutput.split('\n');
    QStringList rSeparatedErrorsList;

    QString separateError;
    for (auto i : errorsOutputLines)
    {
        auto line = i;
        line = line.replace('~',' ');
        line = line.simplified();
        if (line == "^")
        {
            rSeparatedErrorsList.push_back(separateError);
            separateError.clear();
        }
        else
        {
            separateError += line + " ";
        }
    }
    return rSeparatedErrorsList;
}

