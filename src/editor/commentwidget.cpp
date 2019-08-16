#include "commentwidget.h"
#include "ui_commentwidget.h"
#include<QDebug>

CommentWidget::CommentWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CommentWidget)
{
    ui->setupUi(this);
    commentTabWIdget = new QTabWidget;//create the new tabwidget where

    //first tab will be for editing text, setting bold/italic text, writing, etc.
    editTab = new AddCommentTextEdit;
    QVBoxLayout *lay1 = new QVBoxLayout;
    lay1->addWidget(editTab);
    QWidget *tab1 = new QWidget;
    tab1->setLayout(lay1);

    //second tab will be for viewing text which has been formated. This tab only for view
    viewTab = new ViewTextEdit;
    QVBoxLayout *lay2 = new QVBoxLayout;
    lay2->addWidget(viewTab);
    QWidget *tab2 = new QWidget;
    tab2->setLayout(lay2);

    commentTabWIdget->addTab(editTab, "Edit");// 0 tab index
    commentTabWIdget->addTab(viewTab, "View");// 1 tab index

    this->setWindowIcon(QIcon(":/img/comment.ico"));

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(commentTabWIdget);
    this->setLayout(mainLayout);
    //if we switch tab we should set formated text into view tab
    connect(commentTabWIdget, &QTabWidget::currentChanged, this, &CommentWidget::setViewText);
    commentStringForView = editTab->getText();

    this->setEnabled(true);
}

CommentWidget::~CommentWidget()
{
    delete ui;
}

//here we wanna set our comment widget inside codeEditor area and near commentButton(left side of it)
void CommentWidget::setPosition(QPlainTextEdit *editor, AddCommentButton *commentButton)
{
    //calculate the geometry of code editor in the global screen
    auto globalParentPos = editor->mapToGlobal(QPoint(0,0));

    //width should be == 1/2 of the codeEditor width
    //hight should be == 1/3 of the codeEditor hight
    this->setGeometry(globalParentPos.x() + commentButton->x() - editor->width()/2,
                      globalParentPos.y() + commentButton->y(),
                      editor->width() /  2,
                      editor->height() / 3);
}

//regular expresion "re" is the parametr which we use to detect special symbols and all between them.

//in fucntion below we go through add all found match by regex, replace strings with special symbols
//to the string without special symbols and save the position which shows us where we should use special text.
//We delete this symbols but should keep the posiotion, and using these positions in the future we'll be able
//to set special text. We set replaced text(without symbols) ONLY FOR VIEW MODE
void CommentWidget::writeSpecialTextPositions(const QRegularExpression &re, const SpecificTextType &textType)
{
    int oneSideSymbolsCount = textType == SpecificTextType::BOLD ? 2: 1;// number of special symbols by one side ("**" has 2, "_" has 1)
    //firstly we detect all bold match because they can be inside italic. (For example "_**text**_")
    //so, when we detect bold, our text contains "**" symbols. if we detect italic, we should use text where bold symbols("**") are replaced
    QString viewString = textType == SpecificTextType::BOLD ? editTab->getText() : commentStringForView;

    QRegularExpressionMatchIterator matchIter = re.globalMatch(viewString);//find all match by regex

    int shift = 0;// shows us the shift of text we replaced. For exemple we replaced "**text**" to "text"", so all position moved by some shift value
    while (matchIter.hasNext())
    {
        QRegularExpressionMatch match = matchIter.next();// get match
        if (match.hasMatch())
        {
            SpecificText specText;//structute which keeps the start, end positions of special text and the type of this text
            int startOffset = match.capturedStart();//start of matched val
            int endOffset = match.capturedEnd();//end of matched val

            viewString.replace(startOffset - shift,//replace text to the new text without symbols
                               endOffset - startOffset,
                               match.captured().mid(oneSideSymbolsCount,
                                                    match.captured().length() - oneSideSymbolsCount * 2));

            specText.startIndex = startOffset - shift > 0 ? startOffset - shift : 0;//write start of special text position

            shift += oneSideSymbolsCount * 2;//increase shift by value which is equal to count of deleted special symbols
            specText.endIndex = endOffset - shift;//set end of special text position

            specText.textType = textType;// set the type of special symbols
            specificTextVector.push_back(specText);//write

            shiftAllBold(specText, oneSideSymbolsCount);//move all positions of bold
        }
    }
    commentStringForView = viewString;//set formated text to the view field
    viewTab->setText(viewString);//and to the view tab of widget
}

void CommentWidget::setViewText(int index)
{
    if (!index)
    {
        return;
    }
    specificTextVector.clear();//when we got the new text we schould delete all previous positions of this text

    //firsly we should find all bold text matches by regex, write their positions and delete them
    writeSpecialTextPositions(QRegularExpression("\\*\\*(?:(?:[^*])|(?:\\*[^*])|(?:[^*]\\*))*(?:\\*| |\n)*\\*\\*"),
                              SpecificTextType::BOLD);
    //after we schould do the same to italic text
    writeSpecialTextPositions(QRegularExpression("_(.*?)_"), SpecificTextType::ITALIC);

    setSpecificTextView();//when we have gotten the position of all special text, we should set this special text to the viewTab
}

QTabWidget *CommentWidget::getCommentTabWIdget() const
{
    return commentTabWIdget;
}

void CommentWidget::setCommentTabWIdget(QTabWidget *value)
{
    commentTabWIdget = value;
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
    //move all bold positions in the vector. We don't need to do that to the italic text because
    //when we write italic positions we'll not move them in the future(bold considers in the first place)
    //and at this time bold positions are already written
    for (auto &i: specificTextVector)
    {
        if (i.textType != SpecificTextType::BOLD)
        {
            break;
        }
        if (i.startIndex > specText.startIndex)
        {
            i.startIndex -= oneSideSymbolsCount * 2;
            i.endIndex   -= oneSideSymbolsCount * 2;
            if (i.endIndex < specText.endIndex)
            {
              i.startIndex = specText.startIndex;
              i.endIndex = specText.endIndex;
            }
        }
    }
}

void CommentWidget::setSpecificTextView()
{
    //create special text view in the View tab using vector of special text
    QTextCursor cursor(viewTab->getDocument());
    for (auto &i : specificTextVector)
    {
        cursor.setPosition(i.startIndex);
        cursor.movePosition(QTextCursor::Right,
                            QTextCursor::KeepAnchor,
                            i.endIndex - i.startIndex);

        QTextCharFormat format;

        if (i.textType == SpecificTextType::BOLD)
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
