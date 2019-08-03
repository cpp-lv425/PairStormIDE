#ifndef CODEEDITOREVENTS_H
#define CODEEDITOREVENTS_H
#include<QPlainTextEdit>
#include "codeeditor.h"
#include "event.h"

class EventBraceLeft : public Event
{
public:
    virtual void operator()(CodeEditor * codeEditor, QKeyEvent *e) override
    {
        eventInsertSymbol(codeEditor, e, "]");
    }
};

class EventBracketLeft : public Event
{
public:
    virtual void operator()(CodeEditor * codeEditor, QKeyEvent *e) override
    {
        eventInsertSymbol(codeEditor, e, "}");
    }
};

class EventShiftEnter : public Event
{
public:
    virtual void operator()(CodeEditor * codeEditor, QKeyEvent *e) override
    {
         e = new QKeyEvent(e->type(), e->key(), e->modifiers()&Qt::MetaModifier &Qt::KeypadModifier);
    }
};

class EventEnter : public Event
{
public:
    virtual void operator()(CodeEditor * codeEditor, QKeyEvent *e) override
    {
        if(isinsidebracket())
        {
            codeEditor->insertPlainText("\n\n");
            emit autotab();
            tabs.resize(tabs.size()-1);
            codeEditor->insertPlainText(tabs);
            codeEditor->moveCursor(QTextCursor::Up);
            emit autotab();
            codeEditor->insertPlainText(tabs);
            return;
        }
        plainTextPressEvent(codeEditor,e);
        emit autotab();
        codeEditor->insertPlainText(tabs);
    }
};

#endif // CODEEDITOREVENTS_H



/*
    static bool pressSlesh=false;


    if(e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return)
    {
        if(isinsidebracket())
        {
            this->insertPlainText("\n\n");
            emit autotab();
            tabs.resize(tabs.size()-1);
            this->insertPlainText(tabs);
            this->moveCursor(QTextCursor::Up);
            emit autotab();
            this->insertPlainText(tabs);
            return;
        }
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
        pressSlesh=true;
    }
    if(e->key() == Qt::Key_Asterisk && pressSlesh){
        QPlainTextEdit::keyPressEvent(e);
        this->insertPlainText("");
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

    if(e->key() == Qt::Key_Slash && e->modifiers() & Qt::ControlModifier)
    {


        QTextCursor cursor = textCursor();
        if(cursor.hasSelection())
        {
            int start = cursor.selectionStart();
            int finish = cursor.selectionEnd();

            selectText(cursor, start, start + COMMENT_BLOCK_START.size());
            QString begin = cursor.selectedText();

            selectText(cursor, finish - COMMENT_BLOCK_START.size(), finish);
            QString end = cursor.selectedText();

if(begin == COMMENT_BLOCK_START && end == COMMENT_BLOCK_END)
            {
                removeMultilineComment(this, cursor, start, finish);
            }
            else
            {
                insertMultilineComment(this, cursor, start, finish);
            }
        }
        else
        {
            moveCursor(QTextCursor::StartOfLine);
            cursor = textCursor();
            selectText(cursor, cursor.position(), cursor.position() + COMMENT_BLOCK_START.size());
            QString lineBegin = cursor.selectedText();
            if(lineBegin == SINGLE_LINE_COMMENT)
                cursor.removeSelectedText();
            else
                insertPlainText(SINGLE_LINE_COMMENT);
        }

    }

    if((e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return) &&// shift + enter
            ((e->modifiers() & Qt::ShiftModifier) == Qt::ShiftModifier))
    {
        e = new QKeyEvent(e->type(), e->key(), e->modifiers()&Qt::MetaModifier &Qt::KeypadModifier);
        QPlainTextEdit::keyPressEvent(e);
        return;
    }

    if(e->key() == Qt::Key_Space || e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return)
    {
        saveStateInTheHistory();
        QPlainTextEdit::keyPressEvent(e);
        return;
    }


    if((e->key() == Qt::Key_V && e->modifiers() & Qt::ControlModifier))
    {
        saveStateInTheHistory();
        QPlainTextEdit::keyPressEvent(e);
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
        QString text = QString::fromStdString(this->changeManager->undo());
        this->document()->setPlainText(text);
        return;
    }

    if(e->key() == Qt::Key_Y && e->modifiers() & Qt::ControlModifier)
    {

        QString text = QString::fromStdString(this->changeManager->redo());
        this->document()->setPlainText(text);
        return;
    }

    QPlainTextEdit::keyPressEvent(e);
}*/
