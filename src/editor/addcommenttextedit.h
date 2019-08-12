#ifndef ADDCOMMENTTEXTEDIT_H
#define ADDCOMMENTTEXTEDIT_H

#include <QWidget>
#include <QFont>
#include <QString>
#include<addcommentbutton.h>
#include<QPlainTextEdit>
#include<QRegularExpression>
#include<QRegularExpressionMatch>
#include<QRegularExpressionMatchIterator>
#include<QVector>
#include<QTextCursor>
#include<QTextCharFormat>

namespace Ui
{
class AddCommentTextEdit;
}

class AddCommentTextEdit : public QWidget
{
    Q_OBJECT

public:
    explicit AddCommentTextEdit(QWidget *parent = nullptr);
    ~AddCommentTextEdit();

    QString getText();
    QPushButton* getSendButton();

public

slots:
    void setBoldPressed();
    void setItalicPressed();
    void setSpecialSelect(QString sighns);
    void sendComment();

signals:
    void setBySpecialSigns(QString sights);
    void emptyComment();
    void notEmptyComment();

private:
    Ui::AddCommentTextEdit *ui;
};

#endif // ADDCOMMENTTEXTEDIT_H
