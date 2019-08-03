#ifndef EVENT_H
#define EVENT_H
#include "codeeditor.h"

class Event{
public:
    virtual void operator()(CodeEditor * codeEditor, QKeyEvent *e)=0;
    virtual ~Event();
    void eventInsertSymbol(CodeEditor * codeEditor,QKeyEvent *e, QString s);

protected:
    bool isinsidebracket(CodeEditor * codeEditor);
    void plainTextPressEvent(CodeEditor * codeEditor, QKeyEvent *e);

    static QString tabs;

public slots:
    void autotab();
};

#endif // EVENT_H
