#include "codeeditor.h"


void CodeEditor::keyPressEvent(QKeyEvent *e)
{
    if(e->key() == Qt::Key_V && e->modifiers() & Qt::ControlModifier)
        {
            emit ctrlPlusVPressed();
        }
        if(e->key() == Qt::Key_Backspace)
        {
            emit backspacePressed();
        }

        if((e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return))
        {
                emit returnPressed();
        }

        if((e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return) &&
                ((e->modifiers() & Qt::ShiftModifier) == Qt::ShiftModifier))
        {
            emit returnPressed();
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

        if(e->key() == Qt::Key_Control)
        {

            //this->zoomIn(2);
        }
        /*if(e->key()== Qt::Key_)
        {
            QPlainTextEdit::keyPressEvent(e);
            this->insertPlainText(")");
            this->moveCursor(QTextCursor::Left);
            this->verticalScrollBar();
            return;
        }*/

        QPlainTextEdit::keyPressEvent(e);
}
