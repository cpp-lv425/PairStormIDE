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
    void operator()(CodeEditor * codeEditor, QKeyEvent *e) override;
    ~EventBraceLeft() override;
};
//Event for left bracket key press
class EventBracketLeft : public Event
{
public:
    void operator()(CodeEditor * codeEditor, QKeyEvent *e) override;
    ~EventBracketLeft() override;
};
//Event for press '
class EventApostrophe : public Event
{
public:
    void operator()(CodeEditor * codeEditor, QKeyEvent *e) override;
    ~EventApostrophe() override;
};
//Event for press "
class EventQouteDbl : public Event
{
public:
    void operator()(CodeEditor * codeEditor, QKeyEvent *e) override;
    ~EventQouteDbl() override;
};
//Event for press (
class EventParenLeft : public Event
{
public:
    void operator()(CodeEditor * codeEditor, QKeyEvent *e) override;
    ~EventParenLeft() override;
};
//Event for shift+enter key press
class EventShiftEnter : public Event
{
public:
    void operator()(CodeEditor * codeEditor, QKeyEvent *e) override;
    ~EventShiftEnter() override;
};
//Event for enter key press
class EventEnter : public Event
{
public:
     void operator()(CodeEditor * codeEditor, QKeyEvent *e) override;
    ~EventEnter() override;
};
//Event for Ctrl + plus
class  EventCtrlPlus : public Event
{
    void operator()(CodeEditor * codeEditor, QKeyEvent *e) override;
    ~EventCtrlPlus() override;
};
//Event for Ctrl + minus
class  EventCtrlMinus : public Event
{
    void operator()(CodeEditor * codeEditor, QKeyEvent *e) override;
    ~EventCtrlMinus() override;
};
//Event for Ctrl +Z
class  EventCtrlZ : public Event
{
    void operator()(CodeEditor * codeEditor, QKeyEvent *e) override;
    ~EventCtrlZ() override;
};
//Event for Ctrl +Y
class  EventCtrlY : public Event
{
    void operator()(CodeEditor * codeEditor, QKeyEvent *e) override;
    ~EventCtrlY() override;
};
//Event for Slash
class  EventSlash : public Event
{
    void operator()(CodeEditor * codeEditor, QKeyEvent *e) override;
    ~EventSlash() override;
};
//Event for Asterisk
class  EventAsterisk : public Event
{
    void operator()(CodeEditor * codeEditor, QKeyEvent *e) override;
    ~EventAsterisk() override;
};
#endif // CODEEDITOREVENTS_H
