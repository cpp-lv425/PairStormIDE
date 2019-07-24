#ifndef FILEMANAGER_H
#define FILEMANAGER_H

class QString;

class FileManager
{
public:
    FileManager();
    QString loadFile(const QString& fileName);
    void createFile(const QString& fname);
    void writeToFile(const QString& fname,
                     const QString& content);
};

#endif // FILEMANAGER_H
