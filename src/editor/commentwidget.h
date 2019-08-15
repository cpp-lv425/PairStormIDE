#ifndef COMMENTWIDGET_H
#define COMMENTWIDGET_H

#include <QWidget>
#include <QTabWidget>
#include<addcommenttextedit.h>
#include<QVBoxLayout>
#include<QPlainTextEdit>
#include "addcommentbutton.h"
#include<viewtextedit.h>
#include<QPalette>

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
    AddCommentTextEdit *getEditTab() const;
    void setEditTab(AddCommentTextEdit *value);

    ViewTextEdit *getViewTab() const;
    void setViewTab(ViewTextEdit *value);

    QRect getCommentButtonGeometry() const;
    void setCommentButtonGeometry(const QRect &value);

    void setComment(const QString &comment);

    int getCommentLine() const;
    void setCommentLine(int value);

    QTabWidget *getCommentTabWIdget() const;
    void setCommentTabWIdget(QTabWidget *value);

public slots:
    void setWholeText(int);
private:
    Ui::CommentWidget *ui;
    AddCommentTextEdit *editTab;
    ViewTextEdit *viewTab;

    QTabWidget *commentTabWIdget;
    QRect commentButtonGeometry;
    int commentLine;
    QString commentStringForView;
    QVector<SpecificText>specificTextVector;
};

#endif // COMMENTWIDGET_H
