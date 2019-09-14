#include "consoleserviceprovider.h"
#include "consoleliterals.h"
#include "documentmanager.h"
#include <QTextCodec>
#include <QString>
#include <QDir>

ConsoleServiceProvider::ConsoleServiceProvider()
{
    mConsoleProcess = new QProcess(this);
    documentManager = new DocumentManager;
    connect(mConsoleProcess, &QProcess::readyReadStandardOutput,
            this, &ConsoleServiceProvider::setStdOutput);
}

void ConsoleServiceProvider::setStdOutput()
{
    mConsoleProcess->setReadChannel(QProcess::StandardError);
    mConsoleProcess->waitForFinished();
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

void ConsoleServiceProvider::runExecutableFile()
{
    if (!executableFileExists())
    {
        return;
    }
    runConsoleCommand("start " + getExecutableFilePath());
}

bool ConsoleServiceProvider::executableFileExists()
{
    QDir dir(mConsoleProcess->workingDirectory());
    dir.setNameFilters(QStringList() << "*.exe");
    dir.setFilter(QDir::Files);
    return !dir.entryList().isEmpty();
}

void ConsoleServiceProvider::writeToConsole(const QString &command)
{
    emit appendedTextIsReadyToSet(command);
}

QString ConsoleServiceProvider::getExecutableFilePath() const
{
    QDir dir(mConsoleProcess->workingDirectory());
    dir.setNameFilters(QStringList() << "*.exe");
    dir.setFilter(QDir::Files);
    return dir.entryList().first();
}

ConsoleServiceProvider::~ConsoleServiceProvider() = default;
