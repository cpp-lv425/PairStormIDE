#include "autocodecompleter.h"
#include<QDebug>

AutoCodeCompleter::AutoCodeCompleter(const QStringList &completions, QObject *parent):
    QCompleter(completions, parent)
{

}

bool AutoCodeCompleter::eventFilter(QObject *object, QEvent *event)
{
    if (event->type() == QEvent::KeyPress)
    {
        qDebug()<<"here";
       // QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
       // switch (keyEvent->modifiers())
       // {
        //case Qt::Key_Space:
            //if (keyEvent->modifiers().testFlag(Qt::ControlModifier))
           // {
                qDebug()<<"here inside";
                QPlainTextEdit *textEdit = qobject_cast<QPlainTextEdit*>(widget());
                QTextCursor textCursor = textEdit->textCursor();
                textCursor.movePosition(QTextCursor::StartOfWord, QTextCursor::KeepAnchor);
                if (textCursor.selectedText().length() >= mMinCompletionPrefixLength)
                {
                    setCompletionPrefix(textCursor.selectedText());
                    QRect rect = QRect(textEdit->cursorRect().bottomLeft(), QSize(100, 5));
                    complete(rect);
                }
                return QCompleter::eventFilter(object, event);
           // }
            //break;
      //  case Qt::Key_Enter:
       // case Qt::Key_Return:
       // case Qt::Key_Tab:
            if (popup()->isVisible())
            {
                popup()->hide();
                if (popup()->currentIndex().isValid())
                {
                    emit activated(popup()->currentIndex());
                    emit activated(popup()->currentIndex().data(completionRole()).toString());
                }
                return true;
            }
        }
        return QCompleter::eventFilter(object, event);
    }



void AutoCodeCompleter::setMinCompletionPrefixLength(int minCompletionPrefixLength)
{
    mMinCompletionPrefixLength = minCompletionPrefixLength;
}

int AutoCodeCompleter::getMinCompletionPrefixLength() const
{
    return mMinCompletionPrefixLength;
}

AutoCodeCompleter::~AutoCodeCompleter() = default;

