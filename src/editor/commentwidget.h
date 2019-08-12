#ifndef COMMENTWIDGET_H
#define COMMENTWIDGET_H

#include <QWidget>
#include <QTabWidget>
#include<addcommenttextedit.h>
#include<QVBoxLayout>
#include<QPlainTextEdit>
#include "addcommentbutton.h"
#include<viewtextedit.h>

enum SpecificTextType{BOLD,
                      ITALIC};

struct SpecificText
{
    int startIndex;
    int endIndex;
    SpecificTextType textType;
};

namespace Ui
{
class CommentWidget;
}

class CommentWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CommentWidget(QWidget *parent = nullptr);
    ~CommentWidget();
    void setPosition(QPlainTextEdit *editor, AddCommentButton *commentButton);
    void writeSpecialTextPositions(const QRegularExpression &re, const SpecificTextType &textType);
    void shiftAllBold(const SpecificText &specText,const int &oneSideSymbolsCount);
    void setSpecificTextView();
public slots:
        void setWholeText(int);
private:
    Ui::CommentWidget *ui;
    AddCommentTextEdit *editTab;
    ViewTextEdit *viewTab;

    int commentLine;
    QString commentString;
    QVector<SpecificText>specificTextVector;
};

#endif // COMMENTWIDGET_H
