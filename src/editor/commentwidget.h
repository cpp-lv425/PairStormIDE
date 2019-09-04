#ifndef COMMENTWIDGET_H
#define COMMENTWIDGET_H

#include "addcommentbutton.h"
#include "viewtextedit.h"
#include "addcommenttextedit.h"
#include <QWidget>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QPlainTextEdit>

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

    AddCommentTextEdit* getEditTab() const;
    void setEditTab(AddCommentTextEdit *value);

    ViewTextEdit* getViewTab() const;
    void setViewTab(ViewTextEdit *value);

    QRect getCommentButtonGeometry() const;
    void setCommentButtonGeometry(const QRect &value);

    void setComment(const QString &comment);

    int getCommentLine() const;
    void setCommentLine(int value);

    QTabWidget* getCommentTabWIdget() const;
    void setCommentTabWIdget(QTabWidget *value);

private:
    void shiftAllBold(const SpecificText &specText,const int &oneSideSymbolsCount);
    void setSpecificTextView();

public slots:
    void setViewText(int);
    void writeSpecialTextPositions(const QRegularExpression &re, const QString &specialSymbolsOneSide);

private:
    Ui::CommentWidget *ui;
    AddCommentTextEdit *mEditTab;
    ViewTextEdit *mViewTab;

    QTabWidget *mCommentTabWIdget;
    QRect mCommentButtonGeometry;
    int mCommentLine;
    QString mCommentStringForView;
    QVector<SpecificText>mSpecificTextVector;
};

#endif // COMMENTWIDGET_H
