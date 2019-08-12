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
    void setSpecialText(const QRegularExpression &re, const SpecificTextType &fontType);
    void setPosition(QPlainTextEdit *editor, AddCommentButton *commentButton);
    void setSpecificTextView();

private slots:
    void setWholeText();

private:
    void shiftAllBold(const SpecificText &specText,const int &oneSideSymbolsCount);

    Ui::AddCommentTextEdit *ui;
    int commentLine;
    QString commentString;
    QVector<SpecificText>specificTextVector;
};

#endif // ADDCOMMENTTEXTEDIT_H
