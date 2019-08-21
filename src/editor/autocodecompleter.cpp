#include "autocodecompleter.h"
#include<QDebug>
#include<QThread>
#include"keypressevents.h"

AutoCodeCompleter::AutoCodeCompleter(const QStringList &completions, QObject *parent):
    QCompleter(completions, parent)
{
    connect(this, SIGNAL(activated(QString)), this, SLOT(replaceCurrentWord(QString)));
}

/*
1) rewrite signal
2) rewrite function returns
3) rewrite if/else parts (create special fucntions for that)
4) rewrite it as key
*/

/*bool AutoCodeCompleter::eventFilter(QObject *object, QEvent *event)
{
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);

        if (isNotEnterKey(keyEvent))
        {
            if (isUpDownKey(keyEvent))
            {
                return QCompleter::eventFilter(object, event);
            }

            QPlainTextEdit *textEdit = qobject_cast<QPlainTextEdit*>(widget());
            QTextCursor textCursor = textEdit->textCursor();

            textCursor.movePosition(QTextCursor::StartOfWord, QTextCursor::KeepAnchor);;

            if (textCursor.selectedText().length() >= getMinCompletionPrefixLength())
            {
                setCompletionPrefix(textCursor.selectedText());
                QRect rect = QRect(textEdit->cursorRect().bottomLeft(), QSize(100, 5));
                complete(rect);
                if (keyEvent->key() == Qt::Key_Space)
                {
                    popup()->hide();
                }
            }
            return QCompleter::eventFilter(object, event);
        }

        if (popup()->isVisible())
        {
            popup()->hide();
            if (popup()->currentIndex().isValid())
            {
                emit activated(popup()->currentIndex().data(completionRole()).toString());
            }
            return true;
        }
    }
    return QCompleter::eventFilter(object, event);
}*/

bool AutoCodeCompleter::eventFilter(QObject *object, QEvent *event)
{
    QKeyEvent* keyEvent = dynamic_cast<QKeyEvent*>(event);
    if (!keyEvent)
    {
        return QCompleter::eventFilter(object, event);
    }

    if (isNotEnterKey(keyEvent))
    {
        QPlainTextEdit *textEdit = qobject_cast<QPlainTextEdit*>(widget());
        QTextCursor textCursor = textEdit->textCursor();

        textCursor.movePosition(QTextCursor::StartOfWord, QTextCursor::KeepAnchor);

        if (textCursor.selectedText().length() >= getMinCompletionPrefixLength()
                && !isUpDownKey(keyEvent))
        {
            setCompletionPrefix(textCursor.selectedText());
            QRect rect = QRect(textEdit->cursorRect().bottomLeft(), QSize(100, 5));
            complete(rect);
            if (keyEvent->key() == Qt::Key_Space)
            {
                popup()->hide();
            }
        }
        return QCompleter::eventFilter(object, event);;
    }

    if (popup()->isVisible())
    {
        popup()->hide();
        if (popup()->currentIndex().isValid())
        {
            emit activated(popup()->currentIndex().data(completionRole()).toString());
        }
        return true;
    }
    return QCompleter::eventFilter(object, event);
}

void AutoCodeCompleter::setMinCompletionPrefixLength(int minCompletionPrefixLength)
{
    mMinCompletionPrefixLength = minCompletionPrefixLength;
}

void AutoCodeCompleter::replaceCurrentWord(QString text)
{
    QPlainTextEdit *textEdit = qobject_cast<QPlainTextEdit*>(widget());
    QTextCursor textCursor = textEdit->textCursor();
    textCursor.movePosition(QTextCursor::StartOfWord);
    textCursor.movePosition(QTextCursor::EndOfWord, QTextCursor::KeepAnchor);
    textCursor.insertText(text);
    textEdit->setTextCursor(textCursor);
}

int AutoCodeCompleter::getMinCompletionPrefixLength() const
{
    return mMinCompletionPrefixLength;
}

AutoCodeCompleter::~AutoCodeCompleter() = default;

