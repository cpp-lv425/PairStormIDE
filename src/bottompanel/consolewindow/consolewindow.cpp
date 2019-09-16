#include "consolewindow.h"
#include "ui_consolewindow.h"
#include <QGridLayout>

ConsoleWindow::ConsoleWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConsoleWindow)
{
    ui->setupUi(this);
    mpConsoleServiceProvider = new ConsoleServiceProvider;
    mpConsoleView = new ConsoleView;
    QGridLayout *consoleViewGridLayout = new QGridLayout(this);
    consoleViewGridLayout->addWidget(mpConsoleView);
    this->setLayout(consoleViewGridLayout);

    connect(mpConsoleServiceProvider, &ConsoleServiceProvider::processIsReadyToReadStandartOutput,
            mpConsoleView, &QPlainTextEdit::appendPlainText);

    connect(mpConsoleView, &ConsoleView::commandWasInputed,
            mpConsoleServiceProvider, &ConsoleServiceProvider::runConsoleCommand);

    connect(mpConsoleServiceProvider, &ConsoleServiceProvider::appendedTextIsReadyToSet,
            mpConsoleView, &QPlainTextEdit::appendPlainText);

    connect(mpConsoleServiceProvider, &ConsoleServiceProvider::messageAboutErrorsAfterCompilation,
            this, &ConsoleWindow::reSendErrors);
}

ConsoleWindow::~ConsoleWindow()
{
    delete ui;
}

ConsoleServiceProvider* ConsoleWindow::getConsoleServiceProvider() const
{
    return mpConsoleServiceProvider;
}

void ConsoleWindow::setProjectPath(QString path)
{
    mpConsoleServiceProvider->setWorkingDirectory(path);
}

void ConsoleWindow::reSendErrors(QString string)
{
    emit errorsAreOccuredAfterCompilation(string);
}

void ConsoleWindow::runExecutableFile()
{
    mpConsoleServiceProvider->runExecutableFile();
}
