#ifndef ADDCOMMENTBUTTON_H
#define ADDCOMMENTBUTTON_H

#include <QObject>
#include <QPushButton>

class AddCommentButton: public QPushButton
{
    Q_OBJECT

public:
    AddCommentButton(QWidget *parent = nullptr);
    ~AddCommentButton();
    void setCurrentLine(int currentLine);
    int getCurrentLine();

    QString getCommentString() const;
    void setCommentString(const QString &value);

signals:
    void addCommentButtonPressed(int);

protected:
    virtual void mousePressEvent(QMouseEvent *event);

private:
    int mCurrentLine;
    QString mCommentString;
};

#endif // ADDCOMMENTBUTTON_H
