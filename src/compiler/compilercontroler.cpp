#include "compilercontroler.h"
#include <QDirIterator>
#include <filemanager.h>
#include <QDebug>
#include <QString>

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
    getAllSourceFilesFromTheProjectDirectory(mProjectPath);
}

QString CompilerControler::createMakeFileContent(const QString &executibleFileName) const
{
//    FileManager fileManager;
//    fileManager.createFile(mProjectPath + "MakeFile");
    QString rMakeFileContent;
    rMakeFileContent += QString("CC=g++\n\n") +
            QString("CFLAGS=-c -Wall\n\n") +
            QString("all: ") + executibleFileName + "\n\n" +
            executibleFileName + ": ";

    for (auto sourceFile: sourceFilesPathes)
    {
        rMakeFileContent += sourceFile + ".o ";
    }
    rMakeFileContent += "\t$(CC) ";
    for (auto sourceFile: sourceFilesPathes)
    {
        rMakeFileContent += sourceFile + ".o ";
    }
    rMakeFileContent += "o" + executibleFileName + "\n\n";

    for (auto sourceFile: sourceFilesPathes)
    {
        rMakeFileContent += sourceFile + ".o: " +
                sourceFile + ".cpp\n" +
                "\t$(CC) $ (CFLAGS) " + sourceFile + ".cpp\n\n";
    }

    rMakeFileContent += "clean:\n"
                        "\trm -rf *o " + executibleFileName;

    return rMakeFileContent;
}

void CompilerControler::getAllSourceFilesFromTheProjectDirectory(const QString &dirPath)
{
    qDebug()<<"))path(( = "<<mProjectPath;
    QDirIterator it(mProjectPath, QStringList() << "*.cpp", QDir::Files, QDirIterator::Subdirectories);
    while (it.hasNext())
    {
        qDebug()<<it.hasNext();
    }
}

CompilerControler::~CompilerControler() = default;
