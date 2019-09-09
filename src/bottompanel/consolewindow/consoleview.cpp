#include "consoleview.h"
#include "consoleliterals.h"
#include "methodspartsdefinitiongetters.h"

ConsoleView::ConsoleView(QWidget *parent): QPlainTextEdit (parent)
{
}

void ConsoleView::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)
    {
        emit commandWasInputed(getTextByCursor(this->textCursor()));
    }
    QPlainTextEdit::keyPressEvent(event);
}

ConsoleView::~ConsoleView() = default;
