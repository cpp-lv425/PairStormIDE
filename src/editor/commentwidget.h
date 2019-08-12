#ifndef COMMENTWIDGET_H
#define COMMENTWIDGET_H

#include <QWidget>
#include <QTabWidget>
#include<addcommenttextedit.h>
#include<QVBoxLayout>
#include<QPlainTextEdit>
#include "addcommentbutton.h"

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

private:
    Ui::CommentWidget *ui;
};

#endif // COMMENTWIDGET_H
