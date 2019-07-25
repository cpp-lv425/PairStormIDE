#include "codeeditor.h"


void CodeEditor::keyPressEvent(QKeyEvent *e)
{
        if((e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return) &&
                ((e->modifiers() & Qt::ShiftModifier) == Qt::ShiftModifier))
        {
            //emit returnPressed();
            e = new QKeyEvent(e->type(), e->key(), e->modifiers()&Qt::MetaModifier &Qt::KeypadModifier);
        }

        if(e->key() == Qt::Key_BraceLeft)
        {
             QPlainTextEdit::keyPressEvent(e);
            this->insertPlainText("}");
            this->moveCursor(QTextCursor::Left);
            this->verticalScrollBar();
            return;
        }

        if(e->key() == Qt::Key_BracketLeft)
        {
           QPlainTextEdit::keyPressEvent(e);
           this->insertPlainText("]");
           this->moveCursor(QTextCursor::Left);
           this->verticalScrollBar();
           return;
        }

        if((e->key() == Qt::Key_Plus && e->modifiers() & Qt::ControlModifier)
                && currentZoom <=200)
        {
            this->zoomIn(5);
            currentZoom +=5;
            setViewportMargins(lineNumberAreaWidth(), 0,0 ,0);
            return;
        }

        if((e->key() == Qt::Key_Minus && e->modifiers() & Qt::ControlModifier)
                && currentZoom >=50)
        {
            this->zoomOut(5);
            currentZoom -=5;
            setViewportMargins(lineNumberAreaWidth(), 0,0 ,0);
            return;
        }
        QPlainTextEdit::keyPressEvent(e);
}
