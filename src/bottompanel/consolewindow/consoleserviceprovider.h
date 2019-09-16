#ifndef CONSOLESERVICEPROVIDER_H
#define CONSOLESERVICEPROVIDER_H

#include <QObject>
#include <QString>
#include <QProcess>
#include "documentmanager.h"

class ConsoleServiceProvider: public QObject
{
    Q_OBJECT
public:
    ConsoleServiceProvider();
    virtual ~ConsoleServiceProvider();
    QString compilerPath() const;
    void setStdOutput();
    void closeProcessChanale();
    void detatchCompilingProcess();
signals:
    void processIsReadyToReadStandartOutput(QString);
    void appendedTextIsReadyToSet(QString);
    void messageAboutErrorsAfterCompilation(QString);
public slots:
    void runConsoleCommand(QString command);
    void setWorkingDirectory(QString);
    void runExecutableFile();
    bool executableFileExists();

private:
    void writeToConsole(const QString &command);
    QString getExecutableFilePath() const;
    QProcess *mpConsoleProcess;
    DocumentManager *mpDocumentManager;
};

#endif // CONSOLESERVICEPROVIDER_H
