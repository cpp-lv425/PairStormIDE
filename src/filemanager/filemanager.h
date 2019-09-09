#ifndef FILEMANAGER_H
#define FILEMANAGER_H
#include <QString>
extern const char *projectFileExtension;

class FileManager
{
public:    
    QString readFromFile(const QString &fileName);
    void createFile(const QString &fileName);
    void writeToFile(const QString &fileName,
                     const QString &content);
    void createProjectFile(const QString &path);
    bool projectExists(const QString &path);
    bool sourceFileByTheSameNameExists(const QString &headerName);
    QString getProjectFileName();
private:
    static QString projectFileName;
};

#endif // FILEMANAGER_H
