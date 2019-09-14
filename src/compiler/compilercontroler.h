#ifndef COMPILERCONTROLER_H
#define COMPILERCONTROLER_H

#include <QObject>
#include <QString>
#include <QStringList>
#include "consolewindow/consoleserviceprovider.h"

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
    void removeAllExecutableAndObjectsFiles();
    void setConsoleProvider(ConsoleServiceProvider *value);
    QString getCurrentCompilerPath() const;

    ConsoleServiceProvider* getConsoleProvider() const;

private:
    QString getExecutibleFileName() const;
public slots:
    void setCompilerPath(QString compilerPath);
private:
    QString mCompilerPath;
    QStringList sourceFilesPathes;
    QString mProjectPath;
    ConsoleServiceProvider *consoleProvider;
};

#endif // COMPILERCONTROLER_H
