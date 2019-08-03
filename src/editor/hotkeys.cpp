#include "codeeditor.h"
#include <QDebug>
#include <QRegularExpression>
#include<QTextCursor>


const QString SINGLE_LINE_COMMENT = "//";
const QString COMMENT_BLOCK_START = "/*";
const QString COMMENT_BLOCK_END = "*/";

void selectText(QTextCursor &cursor, int start, int end)
{
    cursor.setPosition(start, QTextCursor::MoveAnchor);
    cursor.setPosition(end, QTextCursor::KeepAnchor);
}

void insertMultilineComment(CodeEditor *editor, QTextCursor &cursor, int start, int end)
{
    cursor.setPosition(start);
    editor->setTextCursor(cursor);
    editor->insertPlainText(COMMENT_BLOCK_START);

    cursor.setPosition(end + COMMENT_BLOCK_START.size());
    editor->setTextCursor(cursor);
    editor->insertPlainText(COMMENT_BLOCK_END);
}

void removeMultilineComment(CodeEditor *editor, QTextCursor &cursor, int start, int end)
{
    selectText(cursor, start, start + COMMENT_BLOCK_START.size());
    cursor.removeSelectedText();

    selectText(cursor, end - COMMENT_BLOCK_START.size() - COMMENT_BLOCK_END.size(), end - COMMENT_BLOCK_END.size());
    cursor.removeSelectedText();
}

/*void CodeEditor::eventBracketLeft(QKeyEvent *e)
{
    QPlainTextEdit::keyPressEvent(e);
    this->insertPlainText("]");
    this->moveCursor(QTextCursor::Left);
    this->verticalScrollBar();
}
void CodeEditor::eventBraceLeft(QKeyEvent *e)
{
    QPlainTextEdit::keyPressEvent(e);
    this->insertPlainText("}");
    this->moveCursor(QTextCursor::Left);
    this->verticalScrollBar();
}


void CodeEditor::eventSlash(QKeyEvent *e)
{
    QPlainTextEdit::keyPressEvent(e);
    ispressSlesh=true;
}
void CodeEditor::eventAsterisk(QKeyEvent *e)
{
    QPlainTextEdit::keyPressEvent(e);
    if(ispressSlesh)
    {
        this->insertPlainText("");
        this->moveCursor(QTextCursor::Left);
        this->moveCursor(QTextCursor::Left);
        this->verticalScrollBar();
        //ispressSlesh=false;
    }
}

void CodeEditor::eventShiftEnter(QKeyEvent *e)
{
   e = new QKeyEvent(e->type(), e->key(),
       e->modifiers()&Qt::MetaModifier &Qt::KeypadModifier);
   QPlainTextEdit::keyPressEvent(e);
}
*/





