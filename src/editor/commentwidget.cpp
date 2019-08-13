#include "commentwidget.h"
#include "ui_commentwidget.h"
#include<QDebug>

CommentWidget::CommentWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CommentWidget)
{
    ui->setupUi(this);
    QTabWidget * commentTabWIdget = new QTabWidget;

    editTab = new AddCommentTextEdit;
    QVBoxLayout *lay1 = new QVBoxLayout;
    lay1->addWidget(editTab);
    QWidget *tab1 = new QWidget;
    tab1->setLayout(lay1);

    viewTab = new ViewTextEdit;
    QVBoxLayout *lay2 = new QVBoxLayout;
    lay2->addWidget(viewTab);
    QWidget *tab2 = new QWidget;
    tab2->setLayout(lay2);

    commentTabWIdget->addTab(editTab, "Edit");
    commentTabWIdget->addTab(viewTab, "View");

    this->setWindowIcon(QIcon(":/img/comment.ico"));

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(commentTabWIdget);
    this->setLayout(mainLayout);
    connect(commentTabWIdget, SIGNAL(currentChanged(int)), this, SLOT(setWholeText(int)));
    commentString = editTab->getText();

    this->setEnabled(true);

}

CommentWidget::~CommentWidget()
{
    delete ui;
}

void CommentWidget::setPosition(QPlainTextEdit *editor, AddCommentButton *commentButton)
{
    auto globalParentPos = editor->mapToGlobal(QPoint(0,0));
    this->setGeometry(globalParentPos.x() + commentButton->x() - editor->width()/2,
                      globalParentPos.y() + commentButton->y(),
                      editor->width() /  2,
                      editor->height() / 3);
}

void CommentWidget::writeSpecialTextPositions(const QRegularExpression &re, const SpecificTextType &textType)
{
    int oneSideSymbolsCount = textType == SpecificTextType::BOLD ? 2: 1;
    SpecificText specText;
    QString findString = textType == SpecificTextType::BOLD ? editTab->getText() : commentString;

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

            specText.startIndex = startOffset - shift > 0 ? startOffset - shift : 0;

            shift += oneSideSymbolsCount * 2;
            specText.endIndex = endOffset - shift;

            specText.textType = textType;
            specificTextVector.push_back(specText);

            shiftAllBold(specText, oneSideSymbolsCount);
        }
    }
    commentString = findString;
    viewTab->setText(findString);
}

void CommentWidget::setWholeText(int index)
{
    if (!index)
    {
        return;
    }
    specificTextVector.clear();
    writeSpecialTextPositions(QRegularExpression("\\*\\*(?:(?:[^*])|(?:\\*[^*])|(?:[^*]\\*))*(?:\\*| |\n)*\\*\\*"),
                              SpecificTextType::BOLD);
    writeSpecialTextPositions(QRegularExpression("_(.*?)_"), SpecificTextType::ITALIC);

    setSpecificTextView();
}

int CommentWidget::getCommentLine() const
{
    return commentLine;
}

void CommentWidget::setCommentLine(int value)
{
    commentLine = value;
}

QRect CommentWidget::getCommentButtonGeometry() const
{
    return commentButtonGeometry;
}

void CommentWidget::setCommentButtonGeometry(const QRect &value)
{
    commentButtonGeometry = value;
}

ViewTextEdit *CommentWidget::getViewTab() const
{
    return viewTab;
}

void CommentWidget::setViewTab(ViewTextEdit *value)
{
    viewTab = value;
}

AddCommentTextEdit *CommentWidget::getEditTab() const
{
    return editTab;
}

void CommentWidget::setEditTab(AddCommentTextEdit *value)
{
    editTab = value;
}

void CommentWidget::shiftAllBold(const SpecificText &specText,const int &oneSideSymbolsCount)
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

void CommentWidget::setSpecificTextView()
{
    QTextCursor cursor(viewTab->getDocument());
    for(auto &i : specificTextVector)
    {
        cursor.setPosition(i.startIndex);
        cursor.movePosition(QTextCursor::Right,
                            QTextCursor::KeepAnchor,
                            i.endIndex - i.startIndex);

        QTextCharFormat format;
        if(i.textType == SpecificTextType::BOLD)
        {
            format.setFontWeight(QFont::Bold);
        }
        else
        {
            format.setFontItalic(true);
        }
        cursor.mergeCharFormat(format);
    }
}
