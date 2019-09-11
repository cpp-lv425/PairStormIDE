#include "consolewindow.h"
#include "ui_consolewindow.h"
#include <QGridLayout>
#include <QVBoxLayout>
#include <QDebug>

ConsoleWindow::ConsoleWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConsoleWindow)
{
    ui->setupUi(this);
    consoleServiceProvider = new ConsoleServiceProvider;
    consoleView = new ConsoleView;

    QGridLayout *consoleViewGridLayout = new QGridLayout (this);
    consoleViewGridLayout->addWidget(consoleView);
    this->setLayout(consoleViewGridLayout);

    connect(consoleServiceProvider, &ConsoleServiceProvider::processIsReadyToReadStandartOutput,
            consoleView, &QPlainTextEdit::appendPlainText);

    connect(consoleView, &ConsoleView::commandWasInputed,
            consoleServiceProvider, &ConsoleServiceProvider::runConsoleCommand);

    connect(consoleServiceProvider, &ConsoleServiceProvider::appendedTextIsReadyToSet,
            consoleView, &QPlainTextEdit::appendPlainText);

//    connect(consoleServiceProvider, &ConsoleServiceProvider::errorsAreOccuredAfterCompilation,
//            this, &ConsoleWindow::reSendErrors);
    connect(consoleServiceProvider, &ConsoleServiceProvider::errorsAreOccuredAfterCompilationInCOnsoleProvider,
            this, &ConsoleWindow::reSendErrors);
}

ConsoleWindow::~ConsoleWindow()
{
    delete ui;
}

void ConsoleWindow::setProjectPath(QString path)
{
    consoleServiceProvider->setWorkingDirectory(path);
}

void ConsoleWindow::reSendErrors(QString string)
{
    qDebug()<<"resend!";
    emit errorsAreOccuredAfterCompilation(string);
}
