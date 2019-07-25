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

        /*if((e->key() == Qt::Key_Enter || e->key() == Qt::Key_Plus))
        {
            this->zoomIn(20);
            //return;
             setViewportMargins(lineNumberArea->width(), 0, 0, 0);
             return;
        }

        if((e->key() == Qt::Key_Enter || e->key() == Qt::Key_Minus))
        {
            this->zoomOut(20);
            //return;
             setViewportMargins(lineNumberArea->width(), 0, 0, 0);
             return;
        }*/


            //this->zoomIn(2);
        /*if(e->key()== Qt::Key_)
        {
            QPlainTextEdit::keyPressEvent(e);
            this->insertPlainText(")");
            this->moveCursor(QTextCursor::Left);
            this->verticalScrollBar();
            return;
        }*/
        /*QWheelEvent *wheel = static_cast<QWheelEvent*>(event);
            if(wheel->modifiers() == Qt::ControlModifier)
                if(wheel->delta() > 0)
                {
                    this->zoomIn(20);
                    //this->lineNumberArea->resize(lineNumberAreaWidth()+20, 0);
                     setViewportMargins(lineNumberArea->width(), 0, 0, 0);
                }
                else
                {
                    this->zoomOut(20);
                   //this->lineNumberArea->resize(lineNumberAreaWidth()-20, 0);
                     setViewportMargins(lineNumberArea->width(), 0, 0, 0);*/
        QPlainTextEdit::keyPressEvent(e);
}
