#include "consoleserviceprovider.h"
#include "consoleliterals.h"
#include <QTextCodec>
#include <QDebug>
#include <QtCore>

ConsoleServiceProvider::ConsoleServiceProvider()
{
    mConsoleProcess = new QProcess(this);
    connect(mConsoleProcess, &QProcess::readyReadStandardOutput,
            this, &ConsoleServiceProvider::setStdOutput);
    mConsoleProcess->setWorkingDirectory("C:\\Users\\Petro\\Desktop");//for test
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
    qDebug()<<"Document manager path = "<<documentManager.getCurrentProjectPath();
    qDebug()<<"Working directory path= "<<mConsoleProcess->workingDirectory();
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

void ConsoleServiceProvider::writeToConsole(const QString &command)
{
    emit appendedTextIsReadyToSet(command);
}

ConsoleServiceProvider::~ConsoleServiceProvider() = default;
