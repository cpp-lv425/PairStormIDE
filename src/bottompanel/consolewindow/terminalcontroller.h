#ifndef TERMINALCONTROLLER_H
#define TERMINALCONTROLLER_H

#include <QObject>
#include "terminalview.h"

class TerminalController: public QObject
{
    Q_OBJECT
public:
    TerminalController(TerminalView *terminaView);
    ~TerminalController();
    void setTextToTerminal(const QString &text);
    QString getTextFromTerminal() const;
    void appendTextToTerminal(const QString &text);
private:
    TerminalView *terminalView;
};

#endif // TERMINALCONTROLLER_H
