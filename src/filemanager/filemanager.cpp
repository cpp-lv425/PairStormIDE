#include "filemanager.h"

#include <QTextStream>
#include <QFile>

#include "utils.h"

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
