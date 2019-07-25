#include "codeeditor.h"
#include "linenumberarea.h"
#include<QtGui>
#include<QDebug>
#include<QTextCursor>
#include<QPainter>
static int counter = 0;


CodeEditor::CodeEditor(QWidget *parent) : QPlainTextEdit(parent)
{
    configParam.setConfigParams("Times New Roman", "17", "WHTE");
    setLineWrapMode(QPlainTextEdit::NoWrap);
    lineNumberArea = new LineNumberArea(this);

    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));
    connect(this,SIGNAL(returnPressed()), this, SLOT(updateLineNumberArea()));
    connect(this,SIGNAL(backspacePressed()), this, SLOT(updateLineNumberArea()));
    connect(this,SIGNAL(ctrlPlusVPressed()), this, SLOT(updateLineNumberArea()));
    connect(this,SIGNAL(wheelScroled()),this,SLOT(updateLineNumberArea()));

    QFont font;
    font.setFamily(this->configParam.mTextStyle);
    font.setPointSize(this->configParam.mFontSize);
    font.setBold(false);
    font.setItalic(false);
    this->setFont(font);
    this->setTabStopDistance(TAB_SPACE * fontMetrics().width(QLatin1Char('0')));
    this->lineNumberArea->resize(fontMetrics().width(QLatin1Char('0')) * MAX_SUPPORTED_DIGITS, 0);
    setViewportMargins(lineNumberArea->width(), 0, 0, 0);
}


int CodeEditor::lineNumberAreaWidth()
{
    return this->lineNumberArea->width();
}



void CodeEditor::updateLineNumberArea()
{
    lineNumberArea->update(0,0, lineNumberArea->width(),lineNumberArea->height());

}



void CodeEditor::resizeEvent(QResizeEvent *e)
{
    QRect cr = contentsRect();
    lineNumberArea->setGeometry(QRect(0, 0, lineNumberArea->width(), cr.height()));
    QPlainTextEdit::resizeEvent(e);
}

void CodeEditor::wheelEvent(QWheelEvent *event)
{
    emit wheelScroled();
    /*QWheelEvent *wheel = static_cast<QWheelEvent*>(event);
    if(wheel->modifiers() == Qt::ControlModifier)
        if(wheel->delta() > 0)
        {
            this->zoomIn(20);
            //this->lineNumberArea->resize(lineNumberAreaWidth()+20, 0);
             setViewportMargins(lineNumberArea->width(), 0, 0, 0);
        }
        else
        {
            this->zoomOut(20);
           //this->lineNumberArea->resize(lineNumberAreaWidth()-20, 0);
             setViewportMargins(lineNumberArea->width(), 0, 0, 0);

        }*/
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
    //painter.fillRect(event->rect(), this->configParam.mLineCounterAreaColor);


    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = static_cast<int>(blockBoundingGeometry(block).translated(contentOffset()).top());
    int bottom = top + static_cast<int>(blockBoundingRect(block).height());

    painter.fillRect(0,0, lineNumberArea->width(),this->lineNumberArea->height(), Qt::lightGray);

    painter.fillRect(0,0, lineNumberArea->width(),this->lineNumberArea->height(), configParam.mLineCounterAreaColor);

    while (block.isValid())
    {
        qDebug()<<counter++;
        QString number = QString::number(blockNumber + 1);
        painter.setPen(Qt::black);
        painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(), Qt::AlignRight, number);
        block = block.next();
        int blockHightDiff = bottom - top;
        top = bottom;
        //bottom = top + static_cast<int>(blockBoundingRect(block).height());
        qDebug()<<"diff = "<<blockHightDiff;
        bottom = top + blockHightDiff;
        ++blockNumber;
    }
}
