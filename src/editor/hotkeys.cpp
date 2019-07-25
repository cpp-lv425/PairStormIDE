#include "codeeditor.h"


void CodeEditor::keyPressEvent(QKeyEvent *e)
{
   // QPlainTextEdit::keyPressEvent(e);
   /* switch(e->key()){
    case (Qt::Key_Enter || 16777220):
        if((e->modifiers() & Qt::ShiftModifier) == Qt::ShiftModifier){
            e = new QKeyEvent(e->type(), e->key(), e->modifiers()&Qt::MetaModifier &Qt::KeypadModifier,
                              e->text(), e->isAutoRepeat(), static_cast<ushort>(e->count()));
        }

        break;

    }*/

   if((e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return))
    {
        e = new QKeyEvent(e->type(), e->key(), e->modifiers()&Qt::MetaModifier &Qt::KeypadModifier,
                          e->text(), e->isAutoRepeat(), static_cast<ushort>(e->count()));
    }

    if(e->key() == Qt::Key_BraceLeft)
    {
         QPlainTextEdit::keyPressEvent(e);
        this->insertPlainText("}");
        this->moveCursor(QTextCursor::Left);
        return;
    }

    if(e->key() == Qt::Key_BracketLeft)
    {
        QPlainTextEdit::keyPressEvent(e);
       this->insertPlainText("]");
       this->moveCursor(QTextCursor::Left);
       return;
    }

    QPlainTextEdit::keyPressEvent(e);
}
