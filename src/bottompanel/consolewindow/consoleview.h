#ifndef CONSOLEVIEW_H
#define CONSOLEVIEW_H

#include <QObject>
#include <QWidget>
#include <QPlainTextEdit>
#include <QString>

class ConsoleView: public QPlainTextEdit
{
    Q_OBJECT
public:
    ConsoleView(QWidget *parent = nullptr);
    ~ConsoleView();
signals:
    void commandWasInputed(QString);
    void projectPathWasChanged(QString);
    // QWidget interface
protected:
    virtual void keyPressEvent(QKeyEvent *event);
};

#endif // CONSOLEVIEW_H
