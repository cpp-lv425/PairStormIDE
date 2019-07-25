#include "codeeditor.h"
#include "linenumberarea.h"
#include<QtGui>
#include<QDebug>
#include<QTextCursor>
#include<QPainter>

static int counter = 0;


CodeEditor::CodeEditor(QWidget *parent) : QPlainTextEdit(parent)
{
    this->setLineWrapMode(QPlainTextEdit::NoWrap);
    lineNumberArea = new LineNumberArea(this);
    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
    connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumberArea(QRect, int)));
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));

    updateLineNumberAreaWidth(0);
    highlightCurrentLine();

    QFont font;
    font.setPointSize(12);
    font.setBold(false);
    font.setItalic(false);
    this->setFont(font);
    QFontMetrics metrics(font);
    this->setTabStopDistance(TAB_SPACE * fontMetrics().width(QLatin1Char('0')));
}

int CodeEditor::lineNumberAreaWidth()
{
    int digits = 1;
    //int max = qMax(1, blockCount());
    int countOfBlock = blockCount();//get current number of line
    //qDebug()<<countOfBlock;
    while (countOfBlock >= 10)// go throught all digits
    {
        countOfBlock /= 10;
        ++digits;// get the total number of digits
    }

    int space = fontMetrics().width(QLatin1Char('0')) * digits;// width for the num line.

    return space;//space for line num drawing
}


void CodeEditor::updateLineNumberAreaWidth(int /* newBlockCount */)
{
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);// to move margin (number area is bigger so we need to move text area rightwards)

}



void CodeEditor::updateLineNumberArea(const QRect &rect, int dy)
{
    if (dy)
        lineNumberArea->scroll(10, dy);
    else
        lineNumberArea->update(0, rect.y(), lineNumberAreaWidth(), rect.height());

    if (rect.contains(viewport()->rect()))
        updateLineNumberAreaWidth(0);
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

    if (!isReadOnly()) {
        QTextEdit::ExtraSelection selection;

        QColor lineColor = QColor(Qt::darkRed).lighter(160);

        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }

    setExtraSelections(extraSelections);
}

void CodeEditor::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(lineNumberArea);
    painter.fillRect(event->rect(), Qt::lightGray);

    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = static_cast<int>(blockBoundingGeometry(block).translated(contentOffset()).top()) ;
    int bottom = top + static_cast<int>(blockBoundingRect(block).height());


    while (block.isValid() && top <= event->rect().bottom())
    {
        if (/*block.isVisible() &&*/ bottom >= event->rect().top())
        {
            qDebug()<<counter++;
            QString number = QString::number(blockNumber + 1);
            painter.setPen(Qt::black);
            painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(),
                             Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + static_cast<int>(blockBoundingRect(block).height());
        ++blockNumber;
}}
