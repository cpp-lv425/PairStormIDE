#include "autocodecompleter.h"
#include<QDebug>
#include"keypressevents.h"

AutoCodeCompleter::AutoCodeCompleter(const QStringList &completions, QObject *parent):
    QCompleter(completions, parent)
{
    mMinCompletionPrefixLength = 1;// we will see completetion menu if we pressed more that 1 character
    connect(this, SIGNAL(activated(QString)), this, SLOT(replaceCurrentWord(QString)));
}

AutoCodeCompleter::~AutoCodeCompleter() = default;

void AutoCodeCompleter::createCompletionMenu(QTextCursor &textCursor,
                                             QPlainTextEdit *textEdit,
                                             QKeyEvent *event)
{
    if (textCursor.selectedText().length() < getMinCompletionPrefixLength())// if min length hasn't inputed
    {
        return;
    }
    setCompletionPrefix(textCursor.selectedText());
    QRect rect = QRect(textEdit->cursorRect().bottomLeft(),//create rectangle with possible words
                       QSize(COMPLETION_MENU_WIDTH, COMPLETION_MENU_HIGHT));
    complete(rect);
    //qt considers space as nothing when we're select start of word (see int the eventFilter function)
    //so if we inserted text and pressed space the fucntion textCursor.movePosition(QTextCursor::StartOfWord, QTextCursor::KeepAnchor)
    //will return previous word. and the previous completion word will be shown again. In order to avoid it, here this situation is
    //hadling by catching space keyPress event. When we pressed space, we don't show popup menu.
    if (event->key() == Qt::Key_Space)
    {
        popup()->hide();
    }
}

bool AutoCodeCompleter::eventFilter(QObject *object, QEvent *event)
{
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);

        if (isNotEnterKey(keyEvent))//any key except enter
        {
            if (isUpDownKey(keyEvent))// in order to choose the order of Completion variatns correctly
            {
                return QCompleter::eventFilter(object, event);
            }

            QPlainTextEdit *textEdit = qobject_cast<QPlainTextEdit*>(widget());//create textEdit
            if (!textEdit)
            {
                return false;
            }
            QTextCursor textCursor = textEdit->textCursor();

            textCursor.movePosition(QTextCursor::StartOfWord, QTextCursor::KeepAnchor);//make selection of the previous word

            createCompletionMenu(textCursor,textEdit,keyEvent);

            return QCompleter::eventFilter(object, event);
        }

        if (popup()->isVisible())
        {
            popup()->hide();
            if (popup()->currentIndex().isValid())
            {
                // if complete word is choosen
                emit activated(popup()->currentIndex().data(completionRole()).toString());
            }
            return true;
        }
    }
    return QCompleter::eventFilter(object, event);// handle not key event
}

void AutoCodeCompleter::replaceCurrentWord(const QString word)
{
    QPlainTextEdit *textEdit = qobject_cast<QPlainTextEdit*>(widget());
    if (!textEdit)
    {
        return;
    }
    QTextCursor textCursor = textEdit->textCursor();
    textCursor.movePosition(QTextCursor::StartOfWord);
    textCursor.movePosition(QTextCursor::EndOfWord, QTextCursor::KeepAnchor);//select word by cursor
    textCursor.insertText(word);//replace selected word to string given in the parametr
    textEdit->setTextCursor(textCursor);
}

void AutoCodeCompleter::setMinCompletionPrefixLength(const int minCompletionPrefixLength)
{
    mMinCompletionPrefixLength = minCompletionPrefixLength;
}

int AutoCodeCompleter::getMinCompletionPrefixLength() const
{
    return mMinCompletionPrefixLength;
}
