#include "codeeditor.h"
#include <QRegularExpression>
#include <QTextCursor>

const QString SINGLE_LINE_COMMENT = "//";
const QString COMMENT_BLOCK_START = "/*";
const QString COMMENT_BLOCK_END = "*/";
QString CodeEditor::tabs = "";

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
        {
            return true;
        }
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
        {
            lbrackets++;
        }
        if(text.at(c) == "}")
        {
            rbrackets++;
        }
    }
    tabs = "";
    int difference = lbrackets - rbrackets;

    for(int i = 0; i < difference; i++)
    {
      tabs.append("\t");
    }
}

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

void CodeEditor::keyPressEvent(QKeyEvent *e)
{
    static bool pressSlesh=false;
    if((e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return) &&// shift + enter
            ((e->modifiers() & Qt::ShiftModifier) == Qt::ShiftModifier))
    {
        e = new QKeyEvent(e->type(), e->key(), e->modifiers()&Qt::MetaModifier &Qt::KeypadModifier);
    }

    if(e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return || e->key() == Qt::Key_Space)
    {
        saveStateInTheHistory();
    }

    if(e->key() == Qt::Key_D && e->modifiers() & Qt::ControlModifier)
    {
        QTextCursor cursor = textCursor();
        int position = cursor.position();
        for(auto &it: mTokens)
        {
            if(it.mType == State::KW && it.mBegin <= position && it.mEnd >= position)
            {
                emit sendLexem(it.mName);
            }
        }
    }

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
            && mCurrentZoom <= 150)// forbid to zoom so much
    {
        zoom(1);
        return;
    }

    if((e->key() == Qt::Key_Minus && e->modifiers() & Qt::ControlModifier)//ctrl & -
            && mCurrentZoom >= 50)//forbid to zoom so much
    {
        zoom(-1);
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
            {
                cursor.removeSelectedText();
            }
            else
            {
                insertPlainText(SINGLE_LINE_COMMENT);
            }
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
        QPlainTextEdit::keyPressEvent(e);
        saveStateInTheHistory();
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
            && mCurrentZoom <= 150)// forbid to zoom so much
    {
       zoom(1);
       return;
    }

    if((e->key() == Qt::Key_Minus && e->modifiers() & Qt::ControlModifier)//ctrl & -
            && mCurrentZoom >= 50)//forbid to zoom so much
    {
        zoom(-1);
        return;
    }

    if(e->key() == Qt::Key_Z && e->modifiers() & Qt::ControlModifier)
    {
        undo();
        return;
    }

    if(e->key() == Qt::Key_Y && e->modifiers() & Qt::ControlModifier)
    {
        redo();
        return;
    }
    QPlainTextEdit::keyPressEvent(e);
}
