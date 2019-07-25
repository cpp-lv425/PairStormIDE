#include "filemanager.h"

#include <QTextStream>
#include <QException> // temp
#include <QFile>

QString FileManager::readFromFile(const QString& fname)
{
    QFile file(fname);

    if (file.open(QIODevice::ReadOnly))
    {
        QTextStream stream(&file);
        QString readResult = stream.readAll();
        file.close();
        return readResult;
    }
    throw QException();
}

void FileManager::createFile(const QString& fname)
{
    QFile file(fname);

    if (file.open(QIODevice::WriteOnly))
    {
        file.close();
        return;
    }
    throw QException();
}

void FileManager::writeToFile(const QString &fname,
                             const QString &content)
{
    QFile file(fname);

    if (file.open(QIODevice::WriteOnly))
    {
        QTextStream(&file) << content;
        file.close();
        return;
    }
    throw QException();
}
