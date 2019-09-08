#include "consoleview.h"
#include "consoleliterals.h"
#include "methodspartsdefinitiongetters.h"

ConsoleView::ConsoleView(QWidget *parent): QPlainTextEdit (parent)
{
    this->appendPlainText(commandTextPrefix);
}

void ConsoleView::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)
    {
        emit commandWasInputed(getTextByCursor(this->textCursor()));
        //QPlainTextEdit::keyPressEvent(event);
       // this->appendPlainText(commandTextPrefix);
        //return;
    }
    QPlainTextEdit::keyPressEvent(event);
}

ConsoleView::~ConsoleView() = default;
