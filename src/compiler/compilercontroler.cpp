#include "compilercontroler.h"
#include <QDirIterator>
#include "filemanager.h"
#include <QDebug>
#include <QPlainTextEdit>
#include <consolewindow/consolewindow.h>
#include <QString>
#include<QProcess>

CompilerControler::CompilerControler()
{
}

QString CompilerControler::compilerPath() const
{
    return mCompilerPath;
}

void CompilerControler::setCompilerPath(QString compilerPath)
{
    mCompilerPath = compilerPath;
}

QString CompilerControler::getProjectPath() const
{
    return mProjectPath;
}

void CompilerControler::setProjectPath(const QString &projectPath)
{
    mProjectPath = projectPath;
}

void CompilerControler::runCompilation()
{
    FileManager fileManager;
    auto makeFilePath = mProjectPath + "/MakeFile";

    getAllSourceFilesFromTheProjectDirectory();

    fileManager.createFile(makeFilePath);
    fileManager.writeToFile(makeFilePath, createMakeFileContent(getExecutibleFileName()));

    ConsoleServiceProvider *consoleProvider = new ConsoleServiceProvider;
    consoleProvider->setWorkingDirectory(mProjectPath);
    consoleProvider->runConsoleCommand("mingw32-make.exe -f MakeFile");
    QProcess process;
    QString file = QDir::homePath() + createMakeFileContent(getExecutibleFileName());
    qDebug()<<"FILEEEE = "<<mProjectPath + "/" +  getExecutibleFileName();
   // process

    //run exec
}

QString CompilerControler::getExecutibleFileName() const
{
    auto fullProjectPathParts = mProjectPath.split('/');
    return fullProjectPathParts.last();
}

QString CompilerControler::createMakeFileContent(const QString &executibleFileName) const
{
    QString rMakeFileContent;
    rMakeFileContent += QString("CC=g++\n\n") +
            QString("CFLAGS=-c -Wall\n\n") +
            QString("all: ") + executibleFileName + "\n\n" +
            executibleFileName + ": ";

    for (auto sourceFile: sourceFilesPathes)
    {
        rMakeFileContent += sourceFile + ".o ";
    }
    rMakeFileContent += "\n\t$(CC) ";
    for (auto sourceFile: sourceFilesPathes)
    {
        rMakeFileContent += sourceFile + ".o ";
    }
    rMakeFileContent += "-o " + executibleFileName + "\n\n";

    for (auto sourceFile: sourceFilesPathes)
    {
        rMakeFileContent += sourceFile + ".o: " +
                sourceFile + ".cpp\n" +
                "\t$(CC) $(CFLAGS) " + sourceFile + ".cpp\n\n";
    }

    rMakeFileContent += "clean:\n"
                        "\trm -rf *o " + executibleFileName;

    return rMakeFileContent;
}

void CompilerControler::getAllSourceFilesFromTheProjectDirectory()
{
    QDirIterator fileInDirectoryIter(mProjectPath,
                                    QStringList() << "*.cpp",
                                    QDir::Files,
                                    QDirIterator::Subdirectories);

    while (fileInDirectoryIter.hasNext())
    {
        QFileInfo sourceFileInfo = fileInDirectoryIter.next();
        sourceFilesPathes << sourceFileInfo.completeBaseName();
    }
}

CompilerControler::~CompilerControler() = default;
