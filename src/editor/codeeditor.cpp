#include "codeeditor.h"
#include "linenumberarea.h"
#include "eventbuilder.h"
#include<QtGui>
#include<QTextCursor>
#include<QPainter>
#include<QTextCharFormat>
#include<QFontDatabase>
#include<QScrollBar>
#include<QMessageBox>
#include<iostream>
#include<QLabel>

CodeEditor::CodeEditor(QWidget *parent) : QPlainTextEdit(parent)
{
    setLineWrapMode(QPlainTextEdit::NoWrap);// don't move cursor to the next line where it's out of visible scope
    this->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOn);
    this->setTabStopDistance(TAB_SPACE * fontMetrics().width(QLatin1Char('0')));//set tab distance
    mCurrentZoom = 100;//in persents

    //read settings
    QSettings settings(QApplication::organizationName(), QApplication::applicationName());
    QString analizerFontSize = settings.value("editorFontSize").toString();
    QString analizerFontName = settings.value("editorFontName").toString();
    QString analizerStyle = settings.value("style").toString();
    mConfigParam.setConfigParams(analizerFontName,analizerFontSize,analizerStyle);

    //create objects connected to codeEditor
    mLineNumberArea = new LineNumberArea(this);
    mTimer = new QTimer;
    mLcpp = new LexerCPP();
    mTimer = new QTimer;
    mChangeManager = new ChangeManager(this->toPlainText().toUtf8().constData());
    //comment button
    mAddCommentButton = new AddCommentButton(this);
    mAddCommentButton->setText("+");
    mAddCommentButton->setVisible(false);
    mCurrentCommentLable = new QLabel(this);
    setMouseTracking(true);

    //widget that shows the comment to each line
    mCommentWidget = new CommentWidget;
    mCommentWidget->setVisible(false);


    //This signal is emitted when the text document needs an update of the specified rect.
    //If the text is scrolled, rect will cover the entire viewport area.
    //If the text is scrolled vertically, dy carries the amount of pixels the viewport was scrolled.

    connect(this,                         &QPlainTextEdit::updateRequest,              this, &CodeEditor::updateLineNumberArea);
    connect(mTimer,                       &QTimer::timeout,                            this, &CodeEditor::saveStateInTheHistory);
    connect(this,                         &QPlainTextEdit::cursorPositionChanged,      this, &CodeEditor::textChangedInTheOneLine);
    connect(mAddCommentButton,            &AddCommentButton::addCommentButtonPressed,  this, &CodeEditor::showCommentTextEdit);
    connect(mCommentWidget->getEditTab(), &AddCommentTextEdit::emptyCommentWasSent,    this, &CodeEditor::emptyCommentWasAdded);
    connect(mCommentWidget->getEditTab(), &AddCommentTextEdit::notEmptyCommentWasSent, this, &CodeEditor::notEmptyCommentWasAdded);
    connect(mCommentWidget->getEditTab(), &AddCommentTextEdit::commentWasDeleted,      this, &CodeEditor::deleteComment);
    connect(this,                         &CodeEditor::linesCountUpdated,              this, &CodeEditor::changeCommentButtonsState);
    connect(this,                         &QPlainTextEdit::cursorPositionChanged,      this, &CodeEditor::runLexer);
    connect(this,                         &QPlainTextEdit::cursorPositionChanged,      this, &CodeEditor::highlighText);

    mTimer->start(CHANGE_SAVE_TIME);//save text by this time
    mLinesCountCurrent = 1;
    mLinesCountPrev = 1;

    // start typing from correct position (in the first line it doesn't consider weight of lineCounter)
    //that's why we need to set this position
    updateLineNumberAreaWidth();

    //fonts and colors configurations
    mFont.setPointSize(mConfigParam.mFontSize);
    mFont.setFamily(mConfigParam.mFontStyle);
    mFont.setBold(false);
    mFont.setItalic(false);
    this->setFont(mFont);

    //set text highlighting color
    setTextColors();

    //completer
    QStringList keywords;
    keywords <<"SELECT" <<"FROM" <<"WHERE"<<"WHEN";//for test
    mCompleter = new AutoCodeCompleter(keywords, this);
    mCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    mCompleter->setWidget(this);
    qDebug()<<"inside list:";
    for(auto &i:keywords)
    {
        qDebug()<<i;
    }
}

void CodeEditor::setTextColors()
{
    fmtLiteral.setForeground(mConfigParam.textColors.mStringsColor);
    fmtKeyword.setForeground(mConfigParam.textColors.mBasicLiteralsColor);
    fmtComment.setForeground(mConfigParam.textColors.mCommentColor);
    fmtRegular.setForeground(mConfigParam.textColors.mCodeTextColor);
}

void CodeEditor::setIdeType(const QString &ideType)
{
    mConfigParam.setIdeType(ideType);
    setTextColors();
}

void CodeEditor::setFontSize(const QString &fontSize)
{
    mConfigParam.setFontSize(fontSize);
    mFont.setPointSize(mConfigParam.mFontSize);
}

void CodeEditor::setFontStyle(const QString &fontStyle)
{
    mConfigParam.setFontStyle(fontStyle);
    mFont.setFamily(mConfigParam.mFontStyle);
}


ConfigParams CodeEditor::getConfigParam()
{
    return mConfigParam;
}

void CodeEditor::setConfigParam(const ConfigParams &configParam)
{
    mConfigParam = configParam;
}

void CodeEditor::runLexer()
{
    mLcpp->clear();
    mLcpp->lexicalAnalysis(document()->toPlainText());
    mTokens = mLcpp->getTokens();
}

int CodeEditor::getLineNumberAreaWidth()
{
    int digits = 1;
    int currLineNumber = qMax(1, blockCount());
    while (currLineNumber >= 10)
    {
        currLineNumber /= 10;
        ++digits;
    }
    return fontMetrics().averageCharWidth() * digits;// wight of one symbol(in our case number) * count of digits
}

QString &CodeEditor::getFileName()
{
    return mFileName;
}

void CodeEditor::setFileName(const QString &fileName)
{
    this->mFileName = fileName;
}

std::pair<const QString &, const QString &> CodeEditor::getChangedFileInfo()
{
    return std::make_pair(this->toPlainText(), mFileName);
}

void CodeEditor::undo()
{
    QString text = QString::fromStdString(this->mChangeManager->undo());
    this->document()->setPlainText(text);

    QTextCursor cursor(this->document());
    cursor.setPosition(mChangeManager->getCursorPosPrev());

    this->setTextCursor(cursor);
}

void CodeEditor::redo()
{
    QString text = QString::fromStdString(this->mChangeManager->redo());
    this->document()->setPlainText(text);

    QTextCursor cursor(this->document());
    cursor.setPosition(mChangeManager->getCursorPosNext());

    this->setTextCursor(cursor);
}

bool CodeEditor::isChanged()
{
    return mBeginTextState != this->toPlainText();
}

void CodeEditor::setBeginTextState()
{
    mBeginTextState = this->toPlainText();
}

void CodeEditor::updateLineNumberAreaWidth()
{
    // reset start position for typing (according new linecounter position)
    setViewportMargins(getLineNumberAreaWidth(), 0, 0, 0);
}

void CodeEditor::updateLineNumberArea(const QRect &rect, int dy)// rectangle of current block and Y-Axis changing
{
    if (dy)// when not all of the text is in the visible area (we scrolled it)
    {
        mLineNumberArea->scroll(0, dy);// we should scroll lines numbers in following direction
    }
    else
    {
        mLineNumberArea->update(0, 0, mLineNumberArea->width(), rect.height());//set position to the new block (area for line number)
    }
    if (rect.contains(viewport()->rect()))//when one covers other (text is under line counter)
        updateLineNumberAreaWidth();
}

void CodeEditor::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);
    QRect cr = contentsRect();//whole area inside widget's margins
    mLineNumberArea->setGeometry(QRect(0, 0, getLineNumberAreaWidth(), cr.height()));//set the same height as codeEditor for lineCouter
}

void CodeEditor::specialAreasRepaintEvent(QPaintEvent *event)
{
    QPainter painter(mLineNumberArea);
    qDebug()<<"!!!!!!!!!!"<<mConfigParam.textColors.mLineCounterAreaColor;
    painter.fillRect(event->rect(), mConfigParam.textColors.mLineCounterAreaColor);

    QTextBlock block = firstVisibleBlock();//area of first numeration block from linecounter
    int blockNumber = block.blockNumber();//get line number (start from 0)
    int top = static_cast<int>(blockBoundingGeometry(block).translated(contentOffset()).top());//top of currentblock 0
    int bottom = top + static_cast<int>(blockBoundingRect(block).height());//bottom of current block                 -

    while (block.isValid())//we have blocks (have lines numbers)
    {
        QString number = QString::number(blockNumber + 1);
        painter.setPen(mConfigParam.textColors.mCodeTextColor);
        painter.drawText(0, top, mLineNumberArea->width(), fontMetrics().height(),//draw line count
                         Qt::AlignCenter, number);
        block = block.next();
        int temp = top;//save current top position
        top = bottom;//refresh the top bottom (next block top == this block bottom)
        bottom += bottom - temp;// bottom - temp = dy which is block height
        ++blockNumber;
    }

    repaintButtonsArea(bottom, top, blockNumber);
}

void CodeEditor::repaintButtonsArea(int bottom, int top, int blockNumber)
{
    int addedHight = this->verticalScrollBar()->sliderPosition()? 0 : TOP_UNUSED_PIXELS_HEIGHT;
    int height = bottom - top;
    for (auto &i : mCommentsVector)
    {
        i->setGeometry(this->width() - this->verticalScrollBar()->width() - height,
                       (i->getCurrentLine() - this->verticalScrollBar()->sliderPosition() - 1) * height + addedHight,
                       bottom - top,
                       bottom - top);
    }
    if (mLinesCountCurrent != blockNumber)//lines count changed
    {
        mLinesCountPrev = mLinesCountCurrent;
        mLinesCountCurrent = blockNumber;
        emit linesCountUpdated();
    }
}

void CodeEditor::saveStateInTheHistory()
{
    std::string newFileState = this->toPlainText().toUtf8().constData();
    mChangeManager->writeChange(newFileState);
}

void CodeEditor::zoom(int val)
{
    if (val > 0)
    {
        this->zoomIn(val);
    }
    else
    {
        this->zoomOut(-val);
    }
    mCurrentZoom+=val;
}

void CodeEditor::setZoom(int zoomVal)
{
    zoom(zoomVal - mCurrentZoom);
}

void CodeEditor::textChangedInTheOneLine()
{
    emit(textChangedInLine(this->textCursor().blockNumber() + 1));
}

AddCommentButton *CodeEditor::getCommentButtonByIndex(const int line)
{
    for (int i = 0; i < mCommentsVector.size(); i++)
    {
        if (mCommentsVector[i]->getCurrentLine() == line)
        {
            return mCommentsVector[i];
        }
    }
    return nullptr;
}

void CodeEditor::setNewAddedButtonSettings(AddCommentButton *commentButton)
{
    mCommentWidget->setViewText(1);//set text to view. because it dons't set automiticaly when we set to edit
    commentButton->setCommentString(mCommentWidget->getEditTab()->getText());
    commentButton->setToolTip(mCommentWidget->getViewTab()->getText());
    mCommentWidget->setVisible(false);
}

void CodeEditor::showCommentTextEdit(int line)
{
    mCommentWidget->setPosition(this, mAddCommentButton);
    mCommentWidget->setVisible(true);
    mCommentWidget->setCommentButtonGeometry(mAddCommentButton->geometry());
    mCommentWidget->setCommentLine(line);

    auto commentButton = getCommentButtonByIndex(line);
    if (commentButton)//if comment button by passed in the parametr line existed
    {
        mCommentWidget->getEditTab()->setText(commentButton->getCommentString());//set text from this button text
    }
    else
    {
        mCommentWidget->getEditTab()->setText("");
    }
     QSettings settings(QApplication::organizationName(), QApplication::applicationName());
     mCommentWidget->setWindowTitle("Comment to " + QString::number(line) + " line by " + settings.value("UserName").toString());
    //for text tokens
//    qDebug()<<"tokens vector size = "<<mTokens.size();
//    for (auto &i: mTokens)
//    {
//        if (i.mType == State::ID)
//        {
//            mLexer.getIdentificatorsList()
//            qDebug()<<i.mName;
//        }
//    }
    //
    /*qDebug()<<"vector size = "<<mLexer.getIdentificatorsList().size();
    for (auto &i: mLexer.getIdentificatorsList())
    {
        qDebug()<<i;
    }*/
}

void CodeEditor::emptyCommentWasAdded()
{
    mCommentWidget->setVisible(false);
    auto commentButton = getCommentButtonByIndex(mCommentWidget->getCommentLine());
    if (commentButton)
    {
        removeButtomByValue(mCommentsVector, commentButton);
    }
}

void CodeEditor::notEmptyCommentWasAdded()
{
    if (isCommentButtonExist(mCommentWidget->getCommentLine()))
    {
        auto commentButonExisted = getCommentButtonByIndex(mCommentWidget->getCommentLine());
        setNewAddedButtonSettings(commentButonExisted);
    }
    else
    {
        AddCommentButton *commentButtonNew = new AddCommentButton(this);
        commentButtonNew->setGeometry(mCommentWidget->getCommentButtonGeometry());
        commentButtonNew->setCurrentLine(mCommentWidget->getCommentLine());

        commentButtonNew->setStyleSheet("background-color: #18CD3C");
        commentButtonNew->setText("✔");
        commentButtonNew->setVisible(true);

        QSettings settings(QApplication::organizationName(), QApplication::applicationName());
        commentButtonNew->setUser(settings.value("UserName").toString());

        setNewAddedButtonSettings(commentButtonNew);

        mCommentsVector.push_back(commentButtonNew);
        connect(mCommentsVector.back(), &AddCommentButton::addCommentButtonPressed, this, &CodeEditor::showCommentTextEdit);
    }
}

void CodeEditor::deleteComment()
{
    auto commentButon = getCommentButtonByIndex(mCommentWidget->getCommentLine());
    if (commentButon)
    {
        removeButtomByValue(mCommentsVector, commentButon);
    }
    mCommentWidget->setVisible(false);
}

void CodeEditor::changeCommentButtonsState()
{
    int diff = mLinesCountCurrent - mLinesCountPrev;//number that keeps the difference between previous and current lines count
    int cursorLine = this->textCursor().blockNumber() + 1;//get the line where cursor is

    //get the lines of start and end changing
    int startLine = diff > 0 ? cursorLine - diff : cursorLine;
    int endLine = diff > 0 ? cursorLine : cursorLine - diff;

    removeButtons(mCommentsVector, cursorLine, startLine, endLine, diff);//remove all buttons if we deleted their lines

    rewriteButtonsLines(mCommentsVector,diff,startLine);//move position of buttons that need to be moved
}

LastRemoveKey CodeEditor::getLastRemomeKey() const
{
    return lastRemomeKey;
}

void CodeEditor::setLastRemomeKey(const LastRemoveKey &value)
{
    lastRemomeKey = value;
}

void CodeEditor::rewriteButtonsLines(QVector<AddCommentButton *> &commentV, const int diff, const int startLine)
{
    //move buttons' lines according to diif
    for (auto &i : commentV)
    {
        if (i->getCurrentLine() == startLine)
        {
            auto cursor = this->textCursor();
            cursor.movePosition(QTextCursor::PreviousCharacter);
//check if cursor was in the start of comment line.
//that is the only one situation when we consider this line as the line where we should move comment button
            if (cursor.atBlockStart())
            {
                setAnotherButtonLine(i, diff);
            }
        }
        else if (i->getCurrentLine() > startLine)
        {
            setAnotherButtonLine(i, diff);
        }
    }
}

void CodeEditor::setAnotherButtonLine(AddCommentButton *comment, int diff)
{
    comment->setCurrentLine(comment->getCurrentLine() + diff);
}

bool CodeEditor::isInRangeIncludBoth(int val, int leftMargin, int rightMargin)
{
    return val >= leftMargin && val <= rightMargin;
}

bool CodeEditor::isInRangeIncludLast(int val, int leftMargin, int rightMargin)
{
    return val > leftMargin && val <= rightMargin;
}

void CodeEditor::removeButtonByIndex(QVector<AddCommentButton *> &commentV, int index)
{
    commentV[index]->setVisible(false);
    commentV.erase(commentV.begin() + index);
}

void CodeEditor::removeButtomByValue(QVector<AddCommentButton *> &commentV, AddCommentButton *commentButton)
{
     commentButton->setVisible(false);
     mCommentsVector.erase(std::remove(commentV.begin(), commentV.end(), commentButton), commentV.end());
}

bool CodeEditor::isCommentButtonExist(int line)
{
    for (auto &i : mCommentsVector)
    {
        if (i->getCurrentLine() == line)
        {
            return true;
        }
    }
    return false;
}

void CodeEditor::removeButtons(QVector<AddCommentButton *> &commentV, int cursorLine, int startLine, int endLine, int diff)
{
   if (diff > 0)//we shouldn't remove button if the wasn't any removing
   {
       return;
   }

   for (int i = 0; i < commentV.size(); i++)
   {
       //if the last pressed remove button (delete or backspace) was delete we should check one more condition
       //because cursor position hasn't changed
       if (lastRemomeKey == LastRemoveKey::DEL)
       {
           //this statment happens when we're staying above the line where comment is and pressed Delete.
           //we havn't moved the cursor position, but diff became -1 therefore we can't check this as usually
           //using only startLine position
           if (cursorLine == startLine
                   && cursorLine != commentV[i]->getCurrentLine())
           {
               continue;
           }
           if (isInRangeIncludBoth(commentV[i]->getCurrentLine(), startLine, endLine))
           {
               removeButtonByIndex(commentV, i);//otherwise delete this button (we delete whole line where comment button was)
           }
       }
       else
       {
           if (isInRangeIncludLast(commentV[i]->getCurrentLine(), startLine, endLine))
           {
               removeButtonByIndex(commentV, i);//the same deleting here
           }
       }
   }
}

void CodeEditor::mouseMoveEvent(QMouseEvent *event)
{
    if (event->button() == Qt::NoButton)
    {
        QTextBlock block = this->firstVisibleBlock();

        int top = static_cast<int>(blockBoundingGeometry(block).translated(contentOffset()).top());//top of currentblock 0
        int bottom = top + static_cast<int>(blockBoundingRect(block).height());
        int side = bottom - top;// size of each side of comment button

       // side of rectancge where our bottom will be. X-0 && Y-0 start from the left top, so (top < bottom)
        const QSize buttonSize = QSize(side, side);
        mAddCommentButton->setFixedSize(buttonSize);

        auto currSliderPos = this->verticalScrollBar()->sliderPosition();

        int commentAreaRightMargin = this->width() - this->verticalScrollBar()->width() - getLineNumberAreaWidth();
        int commentAreaLeftMargin = commentAreaRightMargin - side;

       if ((event->x() >= commentAreaLeftMargin) && (event->x() <= commentAreaRightMargin))//mouse inside comment block
       {
            int linesFromTheTop = event->y() / side;
            int currLine = linesFromTheTop + currSliderPos + 1;//because first block = 0
            mAddCommentButton->setCurrentLine(currLine);

            if (currLine <= mLinesCountCurrent)// check if the line exists
            {
                int commentBottonXpos = commentAreaLeftMargin + getLineNumberAreaWidth();
                int commentBottonYpos = linesFromTheTop * side;

                mAddCommentButton->setGeometry(commentBottonXpos, currSliderPos ? commentBottonYpos :
                                          commentBottonYpos + TOP_UNUSED_PIXELS_HEIGHT , side, side);
                mAddCommentButton->setVisible(true);
                //label for line showing
                mCurrentCommentLable->setGeometry(mAddCommentButton->x() - getLineNumberAreaWidth(), mAddCommentButton->y(),
                                                  getLineNumberAreaWidth(), mAddCommentButton->height());
                mCurrentCommentLable->setText(QString::number(mAddCommentButton->getCurrentLine()));
                mCurrentCommentLable->setVisible(true);
            }
       }
       else
       {
           mAddCommentButton->setVisible(false);
           mCurrentCommentLable->setVisible(false);
       }
    }
    QPlainTextEdit::mouseMoveEvent(event);
}

void CodeEditor::closeEvent(QCloseEvent *event)
{
    if (!isChanged())
    {
        event->accept();
        return;
    }
    QMessageBox::StandardButton reply = QMessageBox::question
            (this,
             "Saving Changes",
             "Do you want to save changes to opened documents?",
             QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);

    // if user closes dialog event is ignored
    if (reply == QMessageBox::Cancel)
    {
        event->ignore();
        return;
    }
    // if document wasn't modified of user doesn't want to save changes
    if (reply == QMessageBox::No)
    {
        event->accept();
        return;
    }
    // saving document
    emit closeDocEventOccured(this);
}

void formating(QTextCharFormat fmt, QTextCursor cursor,  Token token)
{
    cursor.setPosition(token.mBegin, QTextCursor::MoveAnchor);
    cursor.setPosition(token.mEnd, QTextCursor::KeepAnchor);
    cursor.setCharFormat(fmt);
}

void CodeEditor::highlighText()
{
    QTextCursor cursor = textCursor();
    for(const auto &i: mTokens)
    {
        switch(i.mType)
        {
        case(State::KW):
            formating(fmtKeyword, cursor, i);
            break;
        case(State::LIT):
            formating(fmtLiteral, cursor, i);
            break;
        case(State::COM):
            formating(fmtComment, cursor, i);
            break;
        default:
            formating(fmtRegular, cursor, i);
            break;
        }
    }
}

void CodeEditor::keyPressEvent(QKeyEvent *e)
{
    Event *pressEvent = EventBuilder::getEvent(e);
    (*pressEvent)(this, e);
}
