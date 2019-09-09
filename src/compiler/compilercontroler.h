#ifndef COMPILERCONTROLER_H
#define COMPILERCONTROLER_H

#include <QObject>
#include <QString>
#include <QStringList>

class CompilerControler: public QObject
{
   // Q_OBJECT
public:
    CompilerControler();
    virtual ~CompilerControler();
    QString compilerPath() const;
    QString createMakeFileContent(const QString &executibleFileName) const;
    void getAllSourceFilesFromTheProjectDirectory(const QString &dirPath);
    QString getProjectPath() const;
    void setProjectPath(const QString &projectPath);
    void runCompilation();

public slots:
    void setCompilerPath(QString compilerPath);
private:
    QString mCompilerPath;
    QStringList sourceFilesPathes;
    QString mProjectPath;
};

#endif // COMPILERCONTROLER_H
