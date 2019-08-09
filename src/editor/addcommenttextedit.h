#ifndef ADDCOMMENTTEXTEDIT_H
#define ADDCOMMENTTEXTEDIT_H

#include <QWidget>

namespace Ui {
class AddCommentTextEdit;
}

class AddCommentTextEdit : public QWidget
{
    Q_OBJECT

public:
    explicit AddCommentTextEdit(QWidget *parent = nullptr);
    ~AddCommentTextEdit();

private slots:
    void on_setFont_clicked();

private:
    Ui::AddCommentTextEdit *ui;
};

#endif // ADDCOMMENTTEXTEDIT_H
