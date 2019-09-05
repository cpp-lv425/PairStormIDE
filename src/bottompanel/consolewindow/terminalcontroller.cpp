#include "terminalcontroller.h"

TerminalController::TerminalController(TerminalView *terminalView)
{
    this->terminalView = terminalView;
}

void TerminalController::setTextToTerminal(const QString &text)
{
    terminalView->getTerminalTextEdit()->setPlainText(text);
}

QString TerminalController::getTextFromTerminal() const
{
    return terminalView->getTerminalTextEdit()->document()->toPlainText();
}

void TerminalController::appendTextToTerminal(const QString &text)
{
    terminalView->getTerminalTextEdit()->appendPlainText(text);
}

TerminalController::~TerminalController() = default;
