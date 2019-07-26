#include "codeeditor.h"
#include <QRegularExpression>

QString CodeEditor::tabs="";

bool CodeEditor::insidebracket(){
    return true;
}


void CodeEditor::autotab(){
    QString text(this->document()->toRawText());
    int lbrackets=0 ;
    int rbrackets=0 ;
    QTextCursor crs=this->textCursor();
    for (int c=0; c<crs.position();c++){
        if(text.at(c)=="{") lbrackets++;
        if(text.at(c)=="}") rbrackets++;
    }
    tabs="";
    int difference=lbrackets-rbrackets;

    for(int i=0; i<difference;i++)
    {
      tabs.append("\t");
    }

}

void CodeEditor::keyPressEvent(QKeyEvent *e)
{
   static bool pressSlesh=false;
        if((e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return) &&// shift + enter
                ((e->modifiers() & Qt::ShiftModifier) == Qt::ShiftModifier))
        {
            e = new QKeyEvent(e->type(), e->key(), e->modifiers()&Qt::MetaModifier &Qt::KeypadModifier);
        }
        if(e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return){
            QPlainTextEdit::keyPressEvent(e);
            emit autotab();
            this->insertPlainText(tabs);
            return;

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
        if(e->key() == Qt::Key_Slash){
           //QPlainTextEdit::keyPressEvent(e);
            pressSlesh=true;
        }
        if(e->key() == Qt::Key_Asterisk && pressSlesh){
            QPlainTextEdit::keyPressEvent(e);
            this->insertPlainText("*/");
            this->moveCursor(QTextCursor::Left);
            this->moveCursor(QTextCursor::Left);
            this->verticalScrollBar();
            return;

        }
        if(e->key() == Qt::Key_ParenLeft)
        {
            QPlainTextEdit::keyPressEvent(e);
            this->insertPlainText(")");
            this->moveCursor(QTextCursor::Left);
            this->verticalScrollBar();
            return;
        }
        if(e->key() == Qt::Key_Apostrophe)
        {
            QPlainTextEdit::keyPressEvent(e);
            this->insertPlainText("\'");
            this->moveCursor(QTextCursor::Left);
            this->verticalScrollBar();
            return;
        }
        if(e->key() == Qt::Key_QuoteDbl)
        {
            QPlainTextEdit::keyPressEvent(e);
            this->insertPlainText("\"");
            this->moveCursor(QTextCursor::Left);
            this->verticalScrollBar();
            return;
        }
        if((e->key() == Qt::Key_Plus && e->modifiers() & Qt::ControlModifier)//ctrl & +
                && currentZoom <= 150)// forbid to zoom so much
        {
            this->zoomIn(1);//increase zoom
            currentZoom += 1;
            setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);// reset text margin in accordance to linecouter change
            return;
        }

        if((e->key() == Qt::Key_Minus && e->modifiers() & Qt::ControlModifier)//ctrl & -
                && currentZoom >= 50)//forbid to zoom so much
        {
            this->zoomOut(1);//decrease zoom
            currentZoom -= 1;
            setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);// reset text margin in accordance to linecouter change
            return;
        }
        QPlainTextEdit::keyPressEvent(e);
}

