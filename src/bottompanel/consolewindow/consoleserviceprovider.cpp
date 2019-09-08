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

void ConsoleServiceProvider::runConsoleCommand(QString command)
{
    QString executionCommand;
    if (QSysInfo::productType() == windowsProductType)
    {
        executionCommand = cmdWindowsStartPath;
    }
    //remove start prefix for command execution (PS$)
   // executionCommand += command.replace(0, QString(commandTextPrefix).length(), QString());
    executionCommand +=command;
    mConsoleProcess->start(executionCommand);
}

void ConsoleServiceProvider::setWorkingDirectory(QString directory)
{
    mConsoleProcess->setWorkingDirectory(directory);
}

void ConsoleServiceProvider::writeToConsole(const QString &command)
{
    emit appendedTextIsReadyToSet(command);
}

ConsoleServiceProvider::~ConsoleServiceProvider() = default;
