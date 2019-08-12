#include "addcommenttextedit.h"
#include "ui_addcommenttextedit.h"
#include <QDebug>

AddCommentTextEdit::AddCommentTextEdit(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddCommentTextEdit)
{
    ui->setupUi(this);
    ui->setBoldButton->setStyleSheet("font-weight: bold");
    ui->setItalicButton->setStyleSheet("font: italic");
    connect(ui->sendMessageButton, SIGNAL(pressed()), this, SLOT(setWholeText()));
}

AddCommentTextEdit::~AddCommentTextEdit()
{
    delete ui;
}

int AddCommentTextEdit::getCommentLine() const
{
    return commentLine;
}

void AddCommentTextEdit::setCommentLine(int value)
{
    commentLine = value;
}

QString AddCommentTextEdit::getCommentString() const
{
    return commentString;
}

void AddCommentTextEdit::setCommentString(const QString &value)
{
    commentString = value;
}

void AddCommentTextEdit::setSpecialText(const QRegularExpression &re, const SpecificTextType &textType)
{
    int oneSideSymbolsCount = textType == SpecificTextType::BOLD ? 2: 1;
    SpecificText specText;
    QString findString = ui->commentTextEdit->toPlainText();
    QRegularExpressionMatchIterator matchIter =  re.globalMatch(findString);
    int shift = 0;
    while(matchIter.hasNext())
    {
        QRegularExpressionMatch match = matchIter.next();
        if (match.hasMatch())
        {
            int startOffset = match.capturedStart();
                        int endOffset = match.capturedEnd();

            findString.replace(startOffset - shift,
                               endOffset - startOffset,
                               match.captured().mid(oneSideSymbolsCount,
                                                    match.captured().length() - oneSideSymbolsCount * 2));

            specText.startIndex = startOffset - shift > 0? startOffset - shift : 0;

            shift += oneSideSymbolsCount * 2;
            specText.endIndex = endOffset - shift;

            specText.textType = textType;
            specificTextVector.push_back(specText);

            shiftAllBold(specText, oneSideSymbolsCount);
        }
    }
    commentString = findString;
    ui->commentTextEdit->setText(findString);
    setSpecificTextView();
}

void AddCommentTextEdit::setWholeText()
{
    specificTextVector.clear();
    setSpecialText(QRegularExpression("\\*\\*(.*?)\\*\\*"), SpecificTextType::BOLD);
    setSpecialText(QRegularExpression("_(.*?)_"), SpecificTextType::ITALIC);
}

void AddCommentTextEdit::shiftAllBold(const SpecificText &specText,const int &oneSideSymbolsCount)
{
    for(auto &i: specificTextVector)
    {
        if(i.textType != SpecificTextType::BOLD)
            break;
        if(i.startIndex > specText.startIndex)
        {
            i.startIndex -= oneSideSymbolsCount * 2;
            i.endIndex   -= oneSideSymbolsCount * 2;
            if(i.endIndex < specText.endIndex)
            {
              i.startIndex = specText.startIndex;
              i.endIndex = specText.endIndex;
            }
        }
    }
}

void AddCommentTextEdit::setSpecificTextView()
{
    QTextCursor cursor(ui->commentTextEdit->document());

    for(auto &i : specificTextVector)
    {
        cursor.setPosition(i.startIndex);
        cursor.movePosition(QTextCursor::Right,
                            QTextCursor::KeepAnchor,
                            i.endIndex - i.startIndex);
        QTextCharFormat format;
        format.setFontWeight(QFont::Bold);
        cursor.mergeCharFormat(format);
        cursor.clearSelection();
//        int times = i.endIndex - i.startIndex;
//        while(times--)
//        {

//        }
    }
}
