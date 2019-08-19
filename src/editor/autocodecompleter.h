#ifndef AUTOCODECOMPLETER_H
#define AUTOCODECOMPLETER_H

#include<QCompleter>
#include<QEvent>
#include<QKeyEvent>
#include<QAbstractItemView>
#include<QTextEdit>
#include<QTextCursor>
#include<QPlainTextEdit>

class AutoCodeCompleter: public QCompleter
{
    Q_OBJECT
public:
    AutoCodeCompleter(const QStringList& completions, QObject *parent = nullptr);
    virtual ~AutoCodeCompleter();

    // QObject interface
public:
    virtual bool eventFilter(QObject *object, QEvent *event);

    int getMinCompletionPrefixLength() const;
    void setMinCompletionPrefixLength(int minCompletionPrefixLength);

private:
    int mMinCompletionPrefixLength = 1;
};

#endif // AUTOCODECOMPLETER_H
