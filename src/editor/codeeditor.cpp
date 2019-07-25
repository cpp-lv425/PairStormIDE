#include "codeeditor.h"
#include "linenumberarea.h"
#include<QtGui>
#include<QDebug>
#include<QTextCursor>
#include<QPainter>

static int counter = 0;


CodeEditor::CodeEditor(QWidget *parent) : QPlainTextEdit(parent)
{

}



void CodeEditor::updateLineNumberAreaWidth(int /* newBlockCount */)
{
}



void CodeEditor::updateLineNumberArea(const QRect &rect, int dy)
{

}



void CodeEditor::resizeEvent(QResizeEvent *e)
{

}



void CodeEditor::highlightCurrentLine()
{

}

void CodeEditor::lineNumberAreaPaintEvent(QPaintEvent *event)
{

}
