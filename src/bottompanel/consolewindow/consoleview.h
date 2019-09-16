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
    ~ConsoleView() override;
signals:
    void commandWasInputed(QString);
    void projectPathWasChanged(QString);
protected:
    virtual void keyPressEvent(QKeyEvent *event) override;
};

#endif // CONSOLEVIEW_H
