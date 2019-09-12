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
signals:
    void processIsReadyToReadStandartOutput(QString);
    void appendedTextIsReadyToSet(QString);
    void messageAboutErrorsAfterCompilation(QString);
public slots:
    void runConsoleCommand(QString command);
    void setWorkingDirectory(QString);
private:
    void writeToConsole(const QString &command);

private:
    QString mCompilerPath;
    QProcess *mConsoleProcess;
    DocumentManager *documentManager;

};

#endif // CONSOLESERVICEPROVIDER_H
