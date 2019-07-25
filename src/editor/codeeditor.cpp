#include "codeeditor.h"
#include "linenumberarea.h"
#include<QtGui>
#include<QDebug>
#include<QTextCursor>
#include<QPainter>
static int counter = 0;


CodeEditor::CodeEditor(QWidget *parent) : QPlainTextEdit(parent)
{
    configParam.setConfigParams("Consolas", "20", "WHTE");
    setLineWrapMode(QPlainTextEdit::NoWrap);
    lineNumberArea = new LineNumberArea(this);

    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
    connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumberArea(QRect,int)));
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));

    updateLineNumberAreaWidth();
    //setViewportMargins(lineNumberArea->width(), 0, 0, 0);
    highlightCurrentLine();
}


int CodeEditor::lineNumberAreaWidth()
{
    int digits = 1;
        int max = qMax(1, blockCount());
        while (max >= 10) {
            max /= 10;
            ++digits;
        }

        int space = 20 + fontMetrics().width(QLatin1Char('0')) * digits;
        return space;
}

void CodeEditor::updateLineNumberAreaWidth()
{
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void CodeEditor::updateLineNumberArea(const QRect &rect, int dy)
{
    if(dy)
    {
        lineNumberArea->scroll(0,dy);
    }
    else
    {
        lineNumberArea->update(0, 0, lineNumberArea->width(), rect.height());
    }
    if(rect.contains(viewport()->rect()))
        updateLineNumberAreaWidth();
}

void CodeEditor::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);
    QRect cr = contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

void CodeEditor::highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;
    QTextEdit::ExtraSelection selection;

    QColor lineColor = configParam.mCurrentLineColor;
    selection.format.setBackground(lineColor);
    selection.format.setProperty(QTextFormat::FullWidthSelection, true);
    selection.cursor = textCursor();
    selection.cursor.clearSelection();
    extraSelections.append(selection);
    setExtraSelections(extraSelections);
}

void CodeEditor::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(lineNumberArea);
    painter.fillRect(event->rect(), Qt::lightGray);


    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = static_cast<int>(blockBoundingGeometry(block).translated(contentOffset()).top());
    int bottom = top + static_cast<int>(blockBoundingRect(block).height());

    while (block.isValid())
    {
        QString number = QString::number(blockNumber + 1);
        painter.setPen(Qt::black);
        painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(),
                         Qt::AlignCenter, number);
        block = block.next();
        int temp = top;
        top = bottom;
        bottom += bottom - temp;
        ++blockNumber;
    }
}
