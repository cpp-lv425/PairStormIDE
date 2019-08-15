#ifndef ADDCOMMENTTEXTEDIT_H
#define ADDCOMMENTTEXTEDIT_H

#include <QFont>
#include <QString>
#include<QPushButton>
#include<QTextCursor>
#include<QRegularExpression>

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
    void setText(const QString &text);
    QPushButton* getSendButton();

public

slots:
    void setBoldPressed();
    void setItalicPressed();
    void setSpecialSelect(QString sighns);
    void sendComment();

signals:
    void setBySpecialSigns(QString sights);
    void emptyCommentWasSent();
    void notEmptyCommentWasSent();

private:
    Ui::AddCommentTextEdit *ui;
};

#endif // ADDCOMMENTTEXTEDIT_H
