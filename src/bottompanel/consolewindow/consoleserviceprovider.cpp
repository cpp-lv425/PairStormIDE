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
         qDebug()<<"process is ready emit"<<codec->toUnicode(mConsoleProcess->readAllStandardOutput());
    }
    else
    {
        emit processIsReadyToReadStandartOutput(mConsoleProcess->readAllStandardOutput());
        qDebug()<<"process is ready emit"<<mConsoleProcess->readAllStandardOutput();
    }
}

void ConsoleServiceProvider::runConsoleCommand(const QString &command)
{
    qDebug()<<"inside runConsoleCommandMethod"<<" command = "<<command;
    QString executionCommand;
    if (QSysInfo::productType() == windowsProductType)
    {
        executionCommand = cmdWindowsStartPath;
    }
    executionCommand += command;
    mConsoleProcess->start(executionCommand);
}

ConsoleServiceProvider::~ConsoleServiceProvider() = default;
