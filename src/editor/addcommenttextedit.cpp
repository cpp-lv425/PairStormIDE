#include "addcommenttextedit.h"
#include "ui_addcommenttextedit.h"
#include<QDebug>

AddCommentTextEdit::AddCommentTextEdit(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddCommentTextEdit)
{
    ui->setupUi(this);
    ui->setBoldButton->setStyleSheet("font-weight: bold");
    ui->setItalicButton->setStyleSheet("font: italic");
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

void AddCommentTextEdit::setCommentToTextEdit()
{
    SpecificText specText;
    qDebug()<<"here";
    QRegularExpression rx("\\*\\*(.*?)\\*\\*");
    QString findString = "**text** **test**donttouchit**00000000000000**";
    QRegularExpressionMatchIterator matchIter =  rx.globalMatch(findString);
    int shift = 0;
    while(matchIter.hasNext())
    {
        QRegularExpressionMatch match = matchIter.next();
        int startOffset = match.capturedStart();
        int endOffset = match.capturedEnd();
        if(match.hasMatch())
        {
            //QString withoutStars = match.captured(0).mid(2,match.captured(0).length() - 4);
           // match.captured(0) = match.captured(0).mid(2,match.captured(0).length() - 4);
           findString.replace(startOffset - shift,endOffset - startOffset, match.captured(0).mid(2,match.captured(0).length() - 4));
            // qDebug()<<"without stars = "<<withoutStars;
            specText.startIndex = startOffset - shift > 0?startOffset - shift:0;
            qDebug()<<"findStr = "<<findString;
            qDebug()<<"start pos = "<<(startOffset - shift > 0?startOffset - shift:0);

            shift+=4;
            specText.endIndex = endOffset - shift;
            specText.textType = SpecificTextType::BOLD;
            specificTextVector.push_back(specText);
            qDebug()<<"end pos = "<<endOffset - shift;
        }
    }
}
void AddCommentTextEdit::setConfiguration(QPlainTextEdit *editor, AddCommentButton *commentButton)
{
    auto globalParentPos = QWidget::mapToGlobal(QPoint(0,0));
    this->setGeometry(globalParentPos.x() + commentButton->x() - editor->width()/2,
                      globalParentPos.y() + commentButton->y(),
                      editor->width() /  2,
                      editor->height() / 3);
}

void AddCommentTextEdit::on_sendMessageButton_clicked()
{
    setCommentToTextEdit();
}
