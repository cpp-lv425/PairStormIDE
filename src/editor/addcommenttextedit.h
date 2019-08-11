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

enum SpecificTextType{BOLD, ITALIC, BOTH};

struct SpecificText
{
    int startIndex;
    int endIndex;
    SpecificTextType textType;
};

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

    int getCommentLine() const;
    void setCommentLine(int value);

    QString getCommentString() const;
    void setCommentString(const QString &value);
    void setCommentToTextEdit();
    void setConfiguration(QPlainTextEdit *editor, AddCommentButton *commentButton);
private slots:


    void on_sendMessageButton_clicked();

private:
    Ui::AddCommentTextEdit *ui;
    int commentLine;
    QString commentString;
    QVector<SpecificText>specificTextVector;
};

#endif // ADDCOMMENTTEXTEDIT_H
