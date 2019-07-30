#include "codeeditor.h"

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

void CodeEditor::keyPressEvent(QKeyEvent *e)
{
        if((e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return) &&// shift + enter
                ((e->modifiers() & Qt::ShiftModifier) == Qt::ShiftModifier))
        {
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
        QPlainTextEdit::keyPressEvent(e);
}
