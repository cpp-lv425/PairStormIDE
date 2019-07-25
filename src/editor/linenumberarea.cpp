#include "linenumberarea.h"

LineNumberArea::LineNumberArea(CodeEditor *editor) : QWidget(editor)
{
}

QSize LineNumberArea:: sizeHint() const
{
   return QSize();
}

void LineNumberArea::paintEvent(QPaintEvent *event)
{
}
