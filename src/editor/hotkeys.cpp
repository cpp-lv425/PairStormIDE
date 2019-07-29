#include "codeeditor.h"
#include<iostream>
#include<string>
#include<QDebug>
void CodeEditor::keyPressEvent(QKeyEvent *e)
{
        if((e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return) &&// shift + enter
                ((e->modifiers() & Qt::ShiftModifier) == Qt::ShiftModifier))
        {
            e = new QKeyEvent(e->type(), e->key(), e->modifiers()&Qt::MetaModifier &Qt::KeypadModifier);

            std::string str = this->toPlainText().toUtf8().constData();
            qDebug()<<"TEXT SET"<<QString::fromStdString(str);
            qDebug()<<"size = "<<changeManager.changesHistory.size();
        }

        if(e->key() == Qt::Key_Space || e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return)
        {
            std::string str = this->toPlainText().toUtf8().constData();
            qDebug()<<"TEXT SET"<<QString::fromStdString(str);
            qDebug()<<"size = "<<changeManager.changesHistory.size();
            changeManager.writeChange(str, this);
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

        if(e->key() == Qt::Key_Z && e->modifiers() & Qt::ControlModifier)
        {
            this->cursor().pos();

            QString text = QString::fromStdString(this->changeManager.undo(this));
            this->document()->setPlainText(text);
            qDebug()<<"size = "<<changeManager.changesHistory.size();
        }

        if(e->key() == Qt::Key_Y && e->modifiers() & Qt::ControlModifier)
        {

            QString text = QString::fromStdString(this->changeManager.redo(this));
           // qDebug()<<"UNDO text = "<<text;
            this->document()->setPlainText(text);
            qDebug()<<"size = "<<changeManager.changesHistory.size();
        }
        QPlainTextEdit::keyPressEvent(e);
}
