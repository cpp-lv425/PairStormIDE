#ifndef AUTOCODECOMPLETER_H
#define AUTOCODECOMPLETER_H

#include<QCompleter>
#include<QEvent>
#include<QKeyEvent>
#include<QAbstractItemView>
#include<QTextEdit>
#include<QTextCursor>
#include<QPlainTextEdit>

const int COMPLETION_MENU_WIDTH  = 100;
const int COMPLETION_MANU_HEIGHT = 5;
//const int

class AutoCodeCompleter: public QCompleter
{
    Q_OBJECT
public:
    AutoCodeCompleter(const QStringList& completions, QObject *parent = nullptr);
    virtual ~AutoCodeCompleter();
private:
    void createCompletionMenu(QTextCursor &textCursor,
                              QPlainTextEdit *textEdit,
                              QKeyEvent *event);
    // QObject interface
public:
    virtual bool eventFilter(QObject *object, QEvent *event);

    int getMinCompletionPrefixLength() const;
    void setMinCompletionPrefixLength(int minCompletionPrefixLength);
public slots:
    void replaceCurrentWord(QString text);

private:
    int mMinCompletionPrefixLength = 1;
};

#endif // AUTOCODECOMPLETER_H
