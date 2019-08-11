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

void AddCommentTextEdit::setSpecialText(const QRegularExpression &re, int oneSideSymbolsCount)
{
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

            specText.textType = oneSideSymbolsCount == 2 ? SpecificTextType::BOLD : SpecificTextType::ITALIC;
            specificTextVector.push_back(specText);
            for(auto &i: specificTextVector)
            {
                if(i.textType == 0)
                {
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
                else
                {
                    break;
                }
            }
        }
        qDebug()<<"str = "<<findString;
        qDebug()<<"vector size = "<<specificTextVector.size();
    }
    for(auto &i: specificTextVector)
    {
        qDebug()<<"start = "<<i.startIndex;
        qDebug()<<"end = "<<i.endIndex;
        qDebug()<<"type = "<<i.textType;
    }
    commentString = findString;
}

void AddCommentTextEdit::setConfiguration(QPlainTextEdit *editor, AddCommentButton *commentButton)
{
    auto globalParentPos = QWidget::mapToGlobal(QPoint(0,0));
    this->setGeometry(globalParentPos.x() + commentButton->x() - editor->width()/2,
                      globalParentPos.y() + commentButton->y(),
                      editor->width() /  2,
                      editor->height() / 3);
}

void AddCommentTextEdit::setWholeText()
{
    specificTextVector.clear();
    setSpecialText(QRegularExpression("\\*\\*(.*?)\\*\\*"), 2);
    setSpecialText(QRegularExpression("_(.*?)_"), 1);
}
