#include "codeeditor.h"
#include "linenumberarea.h"
#include<QtGui>
#include<QDebug>
#include<QTextCursor>
#include<QPainter>


CodeEditor::CodeEditor(QWidget *parent) : QPlainTextEdit(parent)
{
    // just for test. in the future it'll read config parametrs from JSON file
    configParam.setConfigParams("Consolas", "12", "WHITE");
    setLineWrapMode(QPlainTextEdit::NoWrap);// don't move cursor to the next line where it's out of visible scope

    lineNumberArea = new LineNumberArea(this);
    hcpp = new Highlightercpp(document());
    lcpp = new LexerCPP();

    //This signal is emitted when the text document needs an update of the specified rect.
    //If the text is scrolled, rect will cover the entire viewport area.
    //If the text is scrolled vertically, dy carries the amount of pixels the viewport was scrolled.
    connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumberArea(QRect,int)));
    connect(this, SIGNAL(textChanged()), this, SLOT(runLexer()));
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));

    // start typing from correct position (in the first line it doesn't consider weight of lineCounter)
    //that's why we need to set this position
    updateLineNumberAreaWidth();
    highlightCurrentLine();

    //fonts and colors configurations
    font.setPointSize(configParam.mFontSize);
    font.setFamily(configParam.mTextStyle);
    font.setBold(false);
    font.setItalic(false);
    this->setFont(font);
}

void CodeEditor::runLexer()
{
    lcpp->clear();
    lcpp->lexicalAnalysis(document()->toPlainText());
    tokens = lcpp->getTokens();
    for(auto it = tokens.begin(); it < tokens.end(); ++it)
        qDebug() << it->name << " "  << it->begin << " " << it->end << " " << it->linesCount << '\n';
}

int CodeEditor::lineNumberAreaWidth()
{
    int digits = 1;
    int currLineNumber = qMax(1, blockCount());
    while (currLineNumber >= 10)
    {
        currLineNumber /= 10;
        ++digits;
    }

    return fontMetrics().maxWidth() * digits;// wight of one symbol(in our case number) * count of digits
}

void CodeEditor::updateLineNumberAreaWidth()
{
    // reset start position for typing (according new linecounter position)
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void CodeEditor::updateLineNumberArea(const QRect &rect, int dy)// rectangle of current block and Y-Axis changing
{
    if(dy)// when not all of the text is in the visible area (we scrolled it)
    {
        lineNumberArea->scroll(0, dy);// we should scroll lines numbers in following direction
    }
    else
    {
        lineNumberArea->update(0, 0, lineNumberArea->width(), rect.height());//set position to the new block (area for line number)
    }
    if(rect.contains(viewport()->rect()))//when one covers other (text is under line counter)
        updateLineNumberAreaWidth();
}

void CodeEditor::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);
    QRect cr = contentsRect();//whole area inside widget's margins
    lineNumberArea->setGeometry(QRect(0, 0, lineNumberAreaWidth(), cr.height()));//set the same height as codeEditor for lineCouter
}

void CodeEditor::highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;
    QTextEdit::ExtraSelection selection;

    QColor lineColor = configParam.mCurrentLineColor;
    selection.format.setBackground(lineColor);
    selection.format.setProperty(QTextFormat::FullWidthSelection, true);
    selection.cursor = textCursor();
    extraSelections.append(selection);
    setExtraSelections(extraSelections);
}

void CodeEditor::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(lineNumberArea);
    painter.fillRect(event->rect(), configParam.mLineCounterAreaColor);


    QTextBlock block = firstVisibleBlock();//area of first numeration block from linecounter
    int blockNumber = block.blockNumber();//get line number (start from 0)
    int top = static_cast<int>(blockBoundingGeometry(block).translated(contentOffset()).top());//top of currentblock 0
    int bottom = top + static_cast<int>(blockBoundingRect(block).height());//bottom of current block                 -

    while (block.isValid())//we have blocks (have lines numbers)
    {
        QString number = QString::number(blockNumber + 1);
        painter.setPen(configParam.mCodeTextColor);
        painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(),//drat line count
                         Qt::AlignCenter, number);
        block = block.next();
        int temp = top;//save current top position
        top = bottom;//refresh the top bottom (next block top == this block bottom)
        bottom += bottom - temp;// bottom - temp = dy which is block height
        ++blockNumber;
    }
}
