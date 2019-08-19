#include "autocodecompleter.h"
#include<QDebug>

AutoCodeCompleter::AutoCodeCompleter(const QStringList &completions, QObject *parent):
    QCompleter(completions, parent)
{
    connect(this, SIGNAL(activated(QString)), this, SLOT(replaceCurrentWord(QString)));
}

bool AutoCodeCompleter::eventFilter(QObject *object, QEvent *event)
{
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
        switch (keyEvent->key())
        {
        case Qt::Key_Space:
            if (keyEvent->modifiers().testFlag(Qt::ControlModifier))
            {
                QPlainTextEdit *textEdit = qobject_cast<QPlainTextEdit*>(widget());
                QTextCursor textCursor = textEdit->textCursor();
                textCursor.movePosition(QTextCursor::StartOfWord, QTextCursor::KeepAnchor);
                if (textCursor.selectedText().length() >= getMinCompletionPrefixLength())
                {
                    setCompletionPrefix(textCursor.selectedText());
                    QRect rect = QRect(textEdit->cursorRect().bottomLeft(), QSize(100, 5));
                    complete(rect);
                }
                return true;
            }
            break;
        case Qt::Key_Enter:
        case Qt::Key_Return:
        case Qt::Key_Tab:
            if (popup()->isVisible())
            {
                popup()->hide();
                if (popup()->currentIndex().isValid())
                {
                    emit activated(popup()->currentIndex());
                    emit activated(popup()->currentIndex().data(completionRole()).toString());
                    qDebug()<<"emited string = "<<popup()->currentIndex().data(completionRole()).toString();
                }
                return true;
            }
        }
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

