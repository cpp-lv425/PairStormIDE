#ifndef LINENUMBERAREA_H
#define LINENUMBERAREA_H
#include "codeeditor.h"
#include <QWidget>



class LineNumberArea : public QWidget
{
public:
    LineNumberArea(CodeEditor *editor);
    QSize sizeHint() const;

protected:
    void paintEvent(QPaintEvent *event);
private:
    CodeEditor *codeEditor;
};
#endif // LINENUMBERAREA_H
