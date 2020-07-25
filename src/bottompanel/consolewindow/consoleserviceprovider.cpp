#include "consoleserviceprovider.h"
#include "consoleliterals.h"
#include "documentmanager.h"
#include <QTextCodec>
#include <QString>
#include <QDir>

ConsoleServiceProvider::ConsoleServiceProvider()
{
    mpConsoleProcess = new QProcess(this);
    mpDocumentManager = new DocumentManager;
    connect(mpConsoleProcess, &QProcess::readyReadStandardOutput,
            this, &ConsoleServiceProvider::setStdOutput);
}

void ConsoleServiceProvider::setStdOutput()
{
    mpConsoleProcess->setReadChannel(QProcess::StandardError);
    mpConsoleProcess->waitForFinished();
    QString errors = mpConsoleProcess->readAllStandardError();

    if (!errors.isEmpty())
    {
        emit messageAboutErrorsAfterCompilation(errors);
    }
    else
    {
        emit messageAboutNoErrorsAfterCompilation();
    }
    if (QSysInfo::productType() == windowsProductType)// if it's windows' console
    {
        QTextCodec *codec = QTextCodec::codecForName(codecStandart);
        emit processIsReadyToReadStandartOutput
                (codec->toUnicode(mpConsoleProcess->readAllStandardOutput()));
    }
    else
    {
        emit processIsReadyToReadStandartOutput(mpConsoleProcess->readAllStandardOutput());
    }
}

void ConsoleServiceProvider::closeProcessChanale()
{
    mpConsoleProcess->close();
}

void ConsoleServiceProvider::detatchCompilingProcess()
{
    mpConsoleProcess->startDetached();
}

void ConsoleServiceProvider::runConsoleCommand(QString command)
{
    QString executionCommand;
    if (QSysInfo::productType() == windowsProductType)
    {
        executionCommand = cmdWindowsStartPath;
    }
    executionCommand += command;
    mpConsoleProcess->start(executionCommand);
}

void ConsoleServiceProvider::setWorkingDirectory(QString directory)
{
    mpConsoleProcess->setWorkingDirectory(directory);
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
    QDir dir(mpConsoleProcess->workingDirectory());
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
    QDir dir(mpConsoleProcess->workingDirectory());
    dir.setNameFilters(QStringList() << "*.exe");
    dir.setFilter(QDir::Files);
    return dir.entryList().first();
}

ConsoleServiceProvider::~ConsoleServiceProvider() = default;
