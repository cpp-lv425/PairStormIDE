#include "linenumberarea.h"

LineNumberArea::LineNumberArea(CodeEditor *editor) : QWidget(editor)
{
    codeEditor = editor;

}

void LineNumberArea::paintEvent(QPaintEvent *event)
{
    codeEditor->specialAreasRepaintEvent(event);
}
