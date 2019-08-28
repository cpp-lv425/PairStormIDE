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
const int COMPLETION_MENU_HIGHT = 5;

class AutoCodeCompleter: public QCompleter
{
    Q_OBJECT
public:
    AutoCodeCompleter(const QStringList &completions, QObject *parent = nullptr);
    virtual ~AutoCodeCompleter();
    virtual bool eventFilter(QObject *object, QEvent *event);

    int getMinCompletionPrefixLength() const;
    void setMinCompletionPrefixLength(const int minCompletionPrefixLength);
private:
    void createCompletionMenu(QTextCursor &textCursor,
                              QPlainTextEdit *textEdit,
                              QKeyEvent *event);
public slots:
    void replaceCurrentWord(QString word);

private:
    int mMinCompletionPrefixLength;
};

#endif // AUTOCODECOMPLETER_H
