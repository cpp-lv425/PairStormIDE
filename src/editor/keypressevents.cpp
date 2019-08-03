#include "keypressevents.h"

//EventDefault
void EventDefault::operator()(CodeEditor *codeEditor, QKeyEvent *e)
{
        plainTextPressEvent(codeEditor,e);
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
    if(isinsidebracket(codeEditor))
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
    if(editorCurrentZoom(codeEditor) <= 150){
            codeEditor->zoom(1);
    }

}
EventCtrlPlus::~EventCtrlPlus() = default;

//EventCtrlMinus
void EventCtrlMinus::operator()(CodeEditor *codeEditor, QKeyEvent *e)
{
   if(editorCurrentZoom(codeEditor) >= 50)
   {
    codeEditor->zoom(-1);
   }
}
EventCtrlMinus::~EventCtrlMinus() = default;

//EventCtrlZ
void EventCtrlZ::operator()(CodeEditor * codeEditor, QKeyEvent *e)
{
    codeEditor->undo();
}
EventCtrlZ::~EventCtrlZ() = default;

//EventCtrlY
void EventCtrlY::operator()(CodeEditor * codeEditor, QKeyEvent *e)
{
    codeEditor->redo();
}
EventCtrlY::~EventCtrlY() = default;

//EventSlash
void EventSlash::operator()(CodeEditor * codeEditor, QKeyEvent *e)
{
    sIsSleshPressed=true;
    plainTextPressEvent(codeEditor,e);
}
EventSlash::~EventSlash() = default;

//EventAsterisk
void EventAsterisk::operator()(CodeEditor * codeEditor, QKeyEvent *e)
{
    if(sIsSleshPressed)
    {
        eventInsertSymbol(codeEditor, e, "*/");
    }
    else
    {
         plainTextPressEvent(codeEditor,e);
    }
}
EventAsterisk::~EventAsterisk() = default;
