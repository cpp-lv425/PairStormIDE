#ifndef EVENT_H
#define EVENT_H
#include "codeeditor.h"

class Event{
public:
    virtual void operator()(CodeEditor *codeEditor, QKeyEvent *e) = 0;
    virtual ~Event() = default;

protected:
    void eventInsertSymbol(CodeEditor *codeEditor, QKeyEvent *e, QString s);
    static bool IsInsideBracket(CodeEditor *codeEditor);
    static void plainTextPressEvent(CodeEditor *codeEditor, QKeyEvent *e);
    static int editorCurrentZoom(CodeEditor *codeEditor);
    static QList<QVector<Token>> editorTokens(CodeEditor *codeEditor);
    static QString sTabs;
    static bool sIsSleshPressed;

public slots:
    void autotab(CodeEditor *code);
};

#endif // EVENT_H
