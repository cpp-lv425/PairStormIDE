#include "consoleserviceprovider.h"
#include "consoleliterals.h"
#include "documentmanager.h"
#include <QTextCodec>
#include <QDebug>
#include <QtCore>

ConsoleServiceProvider::ConsoleServiceProvider()
{
    mConsoleProcess = new QProcess(this);
    documentManager = new DocumentManager;
    connect(mConsoleProcess, &QProcess::readyReadStandardOutput,
            this, &ConsoleServiceProvider::setStdOutput);
    connect(documentManager, &DocumentManager::projectPathWasChanged,
            this, &ConsoleServiceProvider::setWorkingDirectory);
    //connect(documentManager, SIGNAL(projectPathWasChanged(QString)), mConsoleProcess, SLOT())
    //mConsoleProcess->setWorkingDirectory("C:\\Users\\Petro\\Desktop");//for test
}

QString ConsoleServiceProvider::compilerPath() const
{
    return mCompilerPath;
}

void ConsoleServiceProvider::setCompilerPath(const QString &compilerPath)
{
    mCompilerPath = compilerPath;
}

void ConsoleServiceProvider::setStdOutput()
{
    if (QSysInfo::productType() == windowsProductType)
    {
        QTextCodec *codec = QTextCodec::codecForName(codecStandart);
        emit processIsReadyToReadStandartOutput
                (codec->toUnicode(mConsoleProcess->readAllStandardOutput()));
    }
    else
    {
        emit processIsReadyToReadStandartOutput(mConsoleProcess->readAllStandardOutput());
    }
}

void ConsoleServiceProvider::runConsoleCommand(const QString &command)
{
    QString executionCommand;
    if (QSysInfo::productType() == windowsProductType)
    {
        executionCommand = cmdWindowsStartPath;
    }
    executionCommand += command;
    mConsoleProcess->start(executionCommand);
}

void ConsoleServiceProvider::setWorkingDirectory(QString directory)
{
    qDebug()<<"set working directory";
    mConsoleProcess->setWorkingDirectory(directory);
    qDebug()<<"working directory ="<<directory;
}

void ConsoleServiceProvider::writeToConsole(const QString &command)
{
    emit appendedTextIsReadyToSet(command);
}

ConsoleServiceProvider::~ConsoleServiceProvider() = default;
