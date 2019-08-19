#ifndef FILEMANAGER_H
#define FILEMANAGER_H

class QString;

class FileManager
{
public:    
    QString readFromFile(const QString &fileName);
    void createFile(const QString &fileName);
    void writeToFile(const QString &fileName,
                     const QString &content);
};

#endif // FILEMANAGER_H
