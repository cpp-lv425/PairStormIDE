#ifndef FILEMANAGER_H
#define FILEMANAGER_H

extern const char *projectFileExtension;

class QString;

class FileManager
{
public:    
    QString readFromFile(const QString &fileName);
    void createFile(const QString &fileName);
    void writeToFile(const QString &fileName,
                     const QString &content);
    void createProjectFile(const QString &path);
    bool projectExists(const QString &path);
};

#endif // FILEMANAGER_H
