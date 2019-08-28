#include "filemanager.h"

#include <QTextStream>
#include <QFile>
#include <QDir>

#include "utils.h"

const char *projectFileExtension = ".psproj";
const char pathSeparator = '/';

QString FileManager::readFromFile(const QString &fileName)
{
    if (fileName.isEmpty())
    {
        throw IncorrectUserInput();
    }
    QFile file(fileName);

    if (file.open(QIODevice::ReadOnly))
    {
        QTextStream stream(&file);
        QString rReadResult = stream.readAll();
        file.close();
        return rReadResult;
    }
    throw FileOpeningFailure();
}

void FileManager::createFile(const QString &fileName)
{
    QFile file(fileName);

    if (file.open(QIODevice::WriteOnly))
    {
        file.close();
        return;
    }
    throw FileOpeningFailure();
}

void FileManager::writeToFile(const QString &fileName,
                             const QString &content)
{
    QFile file(fileName);

    if (file.open(QIODevice::WriteOnly))
    {
        QTextStream(&file) << content;
        file.close();
        return;
    }
    throw FileOpeningFailure();
}

void FileManager::createProjectFile(const QString &path)
{
    // constructing full path to project file
    QString projectFileName = path;
    int position = path.lastIndexOf(QChar{pathSeparator});
    projectFileName += pathSeparator;
    projectFileName += path.mid(position + 1);
    projectFileName += projectFileExtension;

    try
    {
        createFile(projectFileName);
    } catch (const FileOpeningFailure&)
    {
        throw;
    }
}

bool FileManager::projectExists(const QString &path)
{
    QDir projectDir(path);

    // construct project file name
    int position = path.lastIndexOf(QChar{pathSeparator});
    QString projectFileName = path.mid(position + 1);
    projectFileName += projectFileExtension;

    // if project file exists in specified directory
    // then true is returned
    return projectDir.exists(projectFileName);
}
