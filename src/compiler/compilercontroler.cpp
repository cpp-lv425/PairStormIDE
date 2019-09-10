#include "compilercontroler.h"
#include <QDirIterator>
#include "filemanager.h"
#include <QDebug>
#include <QPlainTextEdit>
#include <consolewindow/consolewindow.h>
#include <QString>
#include <QThread>
#include<QProcess>

CompilerControler::CompilerControler()
{
    consoleProvider = new ConsoleServiceProvider;
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
    sourceFilesPathes.clear();
    removeAllExecutableAndObjectsFiles();

    FileManager fileManager;
    auto makeFilePath = mProjectPath + "/MakeFile";

    getAllSourceFilesFromTheProjectDirectory();
    for(auto i : sourceFilesPathes)
    {
        qDebug()<<"inside pathes = "<<i;
    }

    fileManager.createFile(makeFilePath);
    fileManager.writeToFile(makeFilePath, createMakeFileContent(getExecutibleFileName()));

    consoleProvider->setWorkingDirectory(mProjectPath);
    consoleProvider->runConsoleCommand("mingw32-make.exe -f MakeFile");
    QString fullExecutableFilePath = mProjectPath + "/" + getExecutibleFileName();
    qDebug()<<"fullExecPath = "<<fullExecutableFilePath;

    QProcess process;

}

QString CompilerControler::getExecutibleFileName() const
{
    auto fullProjectPathParts = mProjectPath.split('/');
    return fullProjectPathParts.last();
}

void CompilerControler::removeAllExecutableAndObjectsFiles()
{
    QDir dir(mProjectPath);
    dir.setNameFilters(QStringList() << "*.o" << "*.exe" << "MakeFile");
    dir.setFilter(QDir::Files);
    foreach (QString dirFile, dir.entryList())
    {
        qDebug()<<"file ="<<dirFile;
        dir.remove(dirFile);
    }
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
