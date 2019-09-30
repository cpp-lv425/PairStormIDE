#ifndef CODEEDITOREVENTS_H
#define CODEEDITOREVENTS_H
#include<QPlainTextEdit>
#include "codeeditor.h"
#include "event.h"

//Event for non-specific key press
class EventDefault: public Event
{
public:
    void operator()(CodeEditor *codeEditor, QKeyEvent *e) override;
    ~EventDefault() override;
};
//Event for left brace key press
class EventBraceLeft : public Event
{
public:
    void operator()(CodeEditor *codeEditor, QKeyEvent *e) override;
    ~EventBraceLeft() override;
};
//Event for left bracket key press
class EventBracketLeft : public Event
{
public:
    void operator()(CodeEditor *codeEditor, QKeyEvent *e) override;
    ~EventBracketLeft() override;
};
//Event for press '
class EventApostrophe : public Event
{
public:
    void operator()(CodeEditor *codeEditor, QKeyEvent *e) override;
    ~EventApostrophe() override;
};
//Event for press "
class EventQouteDbl : public Event
{
public:
    void operator()(CodeEditor *codeEditor, QKeyEvent *e) override;
    ~EventQouteDbl() override;
};
//Event for press (
class EventParenLeft : public Event
{
public:
    void operator()(CodeEditor *codeEditor, QKeyEvent *e) override;
    ~EventParenLeft() override;
};
//Event for shift+enter key press
class EventShiftEnter : public Event
{
public:
    void operator()(CodeEditor *codeEditor, QKeyEvent *e) override;
    ~EventShiftEnter() override;
};
//Event for enter key press
class EventEnter : public Event
{
public:
     void operator()(CodeEditor *codeEditor, QKeyEvent *e) override;
    ~EventEnter() override;
};
//Event for Ctrl + plus
class  EventCtrlPlus : public Event
{
public:
    void operator()(CodeEditor *codeEditor, QKeyEvent *e) override;
    ~EventCtrlPlus() override;
};
//Event for Ctrl + minus
class  EventCtrlMinus : public Event
{
public:
    void operator()(CodeEditor *codeEditor, QKeyEvent *e) override;
    ~EventCtrlMinus() override;
};
//Event for Ctrl +Z
class  EventCtrlZ : public Event
{
public:
    void operator()(CodeEditor *codeEditor, QKeyEvent *e) override;
    ~EventCtrlZ() override;
};
//Event for Ctrl +Y
class  EventCtrlY : public Event
{
public:
    void operator()(CodeEditor *codeEditor, QKeyEvent *e) override;
    ~EventCtrlY() override;
};
//Event for Slash
class  EventSlash : public Event
{
public:
    void operator()(CodeEditor *codeEditor, QKeyEvent *e) override;
    ~EventSlash() override;
};
//Event for Asterisk
class  EventAsterisk : public Event
{
public:
    void operator()(CodeEditor *codeEditor, QKeyEvent *e) override;
    ~EventAsterisk() override;
};
//Event for send lexem to searching info in reference
class  EventSendLexem : public Event
{
public:
    void operator()(CodeEditor *codeEditor, QKeyEvent *e) override;
    ~EventSendLexem() override;

};

//Event for Ctrl + UpArrow
class EventCtrlUpArrow : public Event
{
public:
    void operator()(CodeEditor *codeEditor, QKeyEvent *e) override;
    ~EventCtrlUpArrow() override;
};

//Event for Ctrl + DownArrow
class EventCtrlDownArrow : public Event
{
public:
    void operator()(CodeEditor *codeEditor, QKeyEvent *e) override;
    ~EventCtrlDownArrow() override;
};

//Event for Ctrl + Slash
class  EventCtrlSlash : public Event
{
public:
    void operator()(CodeEditor *codeEditor, QKeyEvent *e) override;

    const QString SINGLE_LINE_COMMENT = "//";
    const QString COMMENT_BLOCK_START = "/*";
    const QString COMMENT_BLOCK_END = "*/";

    static void selectText(QTextCursor &cursor, int start, int end);
    void insertMultilineComment(CodeEditor *editor, QTextCursor &cursor, int start, int end);
    void removeMultilineComment(CodeEditor *editor, QTextCursor &cursor, int start, int end);

    ~EventCtrlSlash() override;
};
//Event for save changes in the history
class  EventSaveChangeInHistory : public Event
{
public:
    void operator()(CodeEditor *codeEditor, QKeyEvent *e) override;
    ~EventSaveChangeInHistory() override;
};

//EventCtrlV
class  EventCtrlV : public Event
{
public:
    void operator()(CodeEditor *codeEditor, QKeyEvent *e) override;
    ~EventCtrlV() override;
};

class EventRemoveKey: public Event
{
public:
    void operator()(CodeEditor *codeEditor, QKeyEvent *e) override;
    ~EventRemoveKey() override;
};

bool isNotEnterKey(QKeyEvent *e);

bool isUpDownKey(QKeyEvent *e);

#endif // CODEEDITOREVENTS_H
