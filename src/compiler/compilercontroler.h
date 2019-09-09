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
    void getAllSourceFilesFromTheProjectDirectory();
    QString getProjectPath() const;
    void setProjectPath(const QString &projectPath);
    void runCompilation();
private:
    QString getExecutibleFileName() const;
public slots:
    void setCompilerPath(QString compilerPath);
private:
    QString mCompilerPath;
    QStringList sourceFilesPathes;
    QString mProjectPath;
};

#endif // COMPILERCONTROLER_H
