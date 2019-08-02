#include "codeeditor.h"

#include <QDebug>
#include <QRegularExpression>
#include<QTextCursor>
#include<QDebug>

QString CodeEditor::tabs="";
bool CodeEditor::ispressSlesh=false;
bool CodeEditor::isinsidebracket()
{
    QTextCursor cursor = this->textCursor();
    if(cursor.position()!=this->document()->toPlainText().size())
    {
        cursor.movePosition(QTextCursor::PreviousCharacter);
        int currentpos = cursor.position();
        QString prev =this->document()->toPlainText().at(currentpos);
        cursor.movePosition(QTextCursor::NextCharacter);
        currentpos = cursor.position();
        QString next = this->document()->toPlainText().at(currentpos);
        if(prev == "{" && next == "}")
            return true;
    }
    return false;
}

void CodeEditor::autotab()
{
    QString text(this->document()->toRawText());
    int lbrackets = 0;
    int rbrackets = 0;
    QTextCursor crs = this->textCursor();
    for (int c = 0; c < crs.position(); c++)
    {
        if(text.at(c) == "{")
            lbrackets++;
        if(text.at(c) == "}")
            rbrackets++;
    }
    tabs = "";
    int difference = lbrackets - rbrackets;

    for(int i = 0; i < difference; i++)
    {
      tabs.append("\t");
    }
}

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

void CodeEditor::eventBracketLeft(QKeyEvent *e)
{

    QPlainTextEdit::keyPressEvent(e);
    this->insertPlainText("]");
    this->moveCursor(QTextCursor::Left);
    this->verticalScrollBar();
    return;
}
void CodeEditor::eventBraceLeft(QKeyEvent *e)
{
    QPlainTextEdit::keyPressEvent(e);
    this->insertPlainText("}");
    this->moveCursor(QTextCursor::Left);
    this->verticalScrollBar();
    return;
}


void CodeEditor::eventSlash(QKeyEvent *e)
{
    QPlainTextEdit::keyPressEvent(e);
    ispressSlesh=true;
    qDebug()<<"sleeshhhh"<<ispressSlesh<<"\n";
    return;
}
void CodeEditor::eventAsterisk(QKeyEvent *e)
{
    QPlainTextEdit::keyPressEvent(e);
    if(ispressSlesh)
    {
        this->insertPlainText("*/");
        this->moveCursor(QTextCursor::Left);
        this->moveCursor(QTextCursor::Left);
        this->verticalScrollBar();
        //ispressSlesh=false;
        return;
    }
}

void CodeEditor::eventShiftEnter(QKeyEvent *e)
{
   e = new QKeyEvent(e->type(), e->key(),
       e->modifiers()&Qt::MetaModifier &Qt::KeypadModifier);
   QPlainTextEdit::keyPressEvent(e);
}

void CodeEditor::keyPressEvent(QKeyEvent *e)
{
   if(mKeysEventMap[qMakePair(e->key(),e->modifiers())])
   {
           (this->*mKeysEventMap.value(qMakePair(e->key(),e->modifiers())))(e);
   }
   else
    QPlainTextEdit::keyPressEvent(e);
}








