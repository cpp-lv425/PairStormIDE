#include "consoleserviceprovider.h"
#include "consoleliterals.h"
#include "documentmanager.h"
#include <QTextCodec>
#include <QString>

ConsoleServiceProvider::ConsoleServiceProvider()
{
    mConsoleProcess = new QProcess(this);
    documentManager = new DocumentManager;
    connect(mConsoleProcess, &QProcess::readyReadStandardOutput,
            this, &ConsoleServiceProvider::setStdOutput);
}

QString ConsoleServiceProvider::compilerPath() const
{
    return mCompilerPath;
}

void ConsoleServiceProvider::setStdOutput()
{
    QString errors = mConsoleProcess->readAllStandardError();
    if (!errors.isEmpty())
    {
        emit messageAboutErrorsAfterCompilation(errors);
    }
    if (QSysInfo::productType() == windowsProductType)// if it's windows' console
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

void ConsoleServiceProvider::closeProcessChanale()
{
    mConsoleProcess->close();
}

void ConsoleServiceProvider::detatchCompilingProcess()
{
    mConsoleProcess->startDetached();
}

void ConsoleServiceProvider::runConsoleCommand(QString command)
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
    mConsoleProcess->setWorkingDirectory(directory);
}

void ConsoleServiceProvider::writeToConsole(const QString &command)
{
    emit appendedTextIsReadyToSet(command);
}

ConsoleServiceProvider::~ConsoleServiceProvider() = default;
