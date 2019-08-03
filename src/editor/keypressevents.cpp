#include "keypressevents.h"

//EventDefault
void EventDefault::operator()(CodeEditor *codeEditor, QKeyEvent *e)
{
        plainTextPressEvent(codeEditor, e);
}
EventDefault::~EventDefault() = default;

//EventBraceLeft
void EventBraceLeft::operator()(CodeEditor *codeEditor, QKeyEvent *e)
{
    eventInsertSymbol(codeEditor, e, "}");
}
EventBraceLeft::~EventBraceLeft() = default;

//EventBracketLeft
void EventBracketLeft::operator()(CodeEditor *codeEditor, QKeyEvent *e)
{
    eventInsertSymbol(codeEditor, e, "]");
}
EventBracketLeft::~EventBracketLeft() = default;

//EventApostrophe
void EventApostrophe::operator()(CodeEditor *codeEditor, QKeyEvent *e)
{
    eventInsertSymbol(codeEditor, e, "'");
}
EventApostrophe::~EventApostrophe() = default;

//EventQuoteDbl
void EventQouteDbl::operator()(CodeEditor *codeEditor, QKeyEvent *e)
{
    eventInsertSymbol(codeEditor, e, "\"");
}
EventQouteDbl::~EventQouteDbl() = default;

//EventParenLeft
void EventParenLeft::operator()(CodeEditor *codeEditor, QKeyEvent *e)
{
    eventInsertSymbol(codeEditor, e, ")");
}
EventParenLeft::~EventParenLeft() = default;

//EventShiftEnter
void EventShiftEnter::operator()(CodeEditor *codeEditor, QKeyEvent *e)
{
    e = new QKeyEvent(e->type(), e->key(), e->modifiers()&Qt::MetaModifier &Qt::KeypadModifier);
    codeEditor->keyPressEvent(e);
}
EventShiftEnter::~EventShiftEnter() = default;

//EventEnter
void EventEnter::operator()(CodeEditor *codeEditor, QKeyEvent *e)
{
    if (isinsidebracket(codeEditor))
    {
        codeEditor->insertPlainText("\n\n");
        emit autotab(codeEditor);
        sTabs.resize(sTabs.size()-1);
        codeEditor->insertPlainText(sTabs);
        codeEditor->moveCursor(QTextCursor::Up);
        emit autotab(codeEditor);
        codeEditor->insertPlainText(sTabs);
        return;
    }
    plainTextPressEvent(codeEditor,e);
    emit autotab(codeEditor);
    codeEditor->insertPlainText(sTabs);
}
EventEnter::~EventEnter() = default;

//EventCtrlPlus
void EventCtrlPlus::operator()(CodeEditor *codeEditor, QKeyEvent *e)
{
    codeEditor->saveStateInTheHistory();
    if (editorCurrentZoom(codeEditor) <= 150)
    {
            codeEditor->zoom(1);
    }

}
EventCtrlPlus::~EventCtrlPlus() = default;

//EventCtrlMinus
void EventCtrlMinus::operator()(CodeEditor *codeEditor, QKeyEvent *e)
{
   if (editorCurrentZoom(codeEditor) >= 50)
   {
    codeEditor->zoom(-1);
   }
}
EventCtrlMinus::~EventCtrlMinus() = default;

//EventCtrlZ
void EventCtrlZ::operator()(CodeEditor *codeEditor, QKeyEvent *e)
{
    codeEditor->undo();
}
EventCtrlZ::~EventCtrlZ() = default;

//EventCtrlY
void EventCtrlY::operator()(CodeEditor *codeEditor, QKeyEvent *e)
{
    codeEditor->redo();
}
EventCtrlY::~EventCtrlY() = default;

//EventSlash
void EventSlash::operator()(CodeEditor *codeEditor, QKeyEvent *e)
{
    sIsSleshPressed=true;
    plainTextPressEvent(codeEditor, e);
}
EventSlash::~EventSlash() = default;

//EventAsterisk
void EventAsterisk::operator()(CodeEditor * codeEditor, QKeyEvent *e)
{
    if (sIsSleshPressed)
    {
        eventInsertSymbol(codeEditor, e, "*/");
    }
    else
    {
         plainTextPressEvent(codeEditor, e);
    }
}
EventAsterisk::~EventAsterisk() = default;

//EventSendLexem
void EventSendLexem::operator()(CodeEditor * codeEditor, QKeyEvent *e)
{
    QTextCursor cursor = codeEditor->textCursor();
    int position = cursor.position();
    for (auto &it: editorTokens(codeEditor))
    {
        if (it.mType == State::KW && it.mBegin <= position && it.mEnd >= position)
        {
            emit codeEditor->sendLexem(it.mName);
        }
    }
}
EventSendLexem::~EventSendLexem() = default;

//EventCtrlSlash
void EventCtrlSlash::operator()(CodeEditor *codeEditor, QKeyEvent *e)
{
    QTextCursor cursor = codeEditor->textCursor();
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
                    removeMultilineComment(codeEditor, cursor, start, finish);
                }
                else
                {
                    insertMultilineComment(codeEditor, cursor, start, finish);
                }
            }
            else
            {
                codeEditor->moveCursor(QTextCursor::StartOfLine);
                cursor = codeEditor->textCursor();
                selectText(cursor, cursor.position(), cursor.position() + COMMENT_BLOCK_START.size());
                QString lineBegin = cursor.selectedText();
                if(lineBegin == SINGLE_LINE_COMMENT)
                {
                    cursor.removeSelectedText();
                }
                else
                {
                    codeEditor->insertPlainText(SINGLE_LINE_COMMENT);
                }
            }
}
void EventCtrlSlash::selectText(QTextCursor &cursor, int start, int end)
{
    cursor.setPosition(start, QTextCursor::MoveAnchor);
    cursor.setPosition(end, QTextCursor::KeepAnchor);
}
void EventCtrlSlash::insertMultilineComment(CodeEditor *editor, QTextCursor &cursor, int start, int end)
{
    cursor.setPosition(start);
    editor->setTextCursor(cursor);
    editor->insertPlainText(COMMENT_BLOCK_START);

    cursor.setPosition(end + COMMENT_BLOCK_START.size());
    editor->setTextCursor(cursor);
    editor->insertPlainText(COMMENT_BLOCK_END);
}
void EventCtrlSlash::removeMultilineComment(CodeEditor *editor, QTextCursor &cursor, int start, int end)
{
    selectText(cursor, start, start + COMMENT_BLOCK_START.size());
    cursor.removeSelectedText();

    selectText(cursor, end - COMMENT_BLOCK_START.size() - COMMENT_BLOCK_END.size(), end - COMMENT_BLOCK_END.size());
    cursor.removeSelectedText();
}
EventCtrlSlash::~EventCtrlSlash() = default;

//EventSaveChangeInHistory
void EventSaveChangeInHistory::operator()(CodeEditor *codeEditor, QKeyEvent *e)
{
    plainTextPressEvent(codeEditor, e);
    codeEditor->saveStateInTheHistory();

}
EventSaveChangeInHistory::~EventSaveChangeInHistory() = default;
