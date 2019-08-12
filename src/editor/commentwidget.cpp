#include "commentwidget.h"
#include "ui_commentwidget.h"

CommentWidget::CommentWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CommentWidget)
{
    ui->setupUi(this);
    QTabWidget * commentTabWIdget = new QTabWidget;

    AddCommentTextEdit *editTab = new AddCommentTextEdit;
    QVBoxLayout *lay1 = new QVBoxLayout;
    lay1->addWidget(editTab);
    QWidget *tab1 = new QWidget;
    tab1->setLayout(lay1);

    QPlainTextEdit *viewTab = new QPlainTextEdit;
    QVBoxLayout *lay2 = new QVBoxLayout;
    lay2->addWidget(viewTab);
    QWidget *tab2 = new QWidget;
    tab2->setLayout(lay2);

    commentTabWIdget->addTab(editTab, "Edit");
    commentTabWIdget->addTab(viewTab, "View");

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(commentTabWIdget);
    this->setLayout(mainLayout);
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
