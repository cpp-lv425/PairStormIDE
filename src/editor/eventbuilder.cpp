#include "eventbuilder.h"

Event *EventBuilder::getEventNoModifier(QKeyEvent *e)
{
    if (e->key() == Qt::Key_BracketLeft)
    {
        return new EventBracketLeft();
    }
    if (e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return)
    {
        return new EventEnter();
    }
    if (e->key() == Qt::Key_Apostrophe)
    {
        return new EventApostrophe();
    }
    if (e->key() == Qt::Key_Slash)
    {
        return new EventSlash();
    }
    return new EventDefault();
}

Event *EventBuilder::getEventShiftModifier(QKeyEvent *e)
{
    if (e->key()==Qt::Key_BraceLeft)
    {
        return new EventBraceLeft();
    }
    if (e->key() == Qt::Key_ParenLeft)
    {
        return new EventParenLeft();
    }
    if (e->key() == Qt::Key_QuoteDbl)
    {
        return new EventQouteDbl();
    }
    if (e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return)
    {
        return new EventShiftEnter();
    }
    if (e->key() == Qt::Key_Asterisk)
    {
        return new EventAsterisk();
    }
    if (e->key() == Qt::Key_Space || e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return)
    {
        return new EventSaveChangeInHistory();
    }
    return new EventDefault();
}

Event *EventBuilder::getEventControlModifier(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Plus)
    {
        return new EventCtrlPlus();
    }
    if (e->key() == Qt::Key_Minus)
    {
        return new EventCtrlMinus();
    }
    if (e->key() == Qt::Key_Z)
    {
        return new EventCtrlZ();
    }
    if (e->key() == Qt::Key_Y)
    {
        return new EventCtrlY();
    }
    if (e->key() == Qt::Key_Slash)
    {
        return new EventCtrlSlash();
    }
    if (e->key() == Qt::Key_D)
    {
        return new EventSendLexem();
    }
    if (e->key() == Qt::Key_V)
    {
        return new EventSaveChangeInHistory();
    }
    return new EventDefault();
}

Event *EventBuilder::getEventKeyboardModifier(QKeyEvent *e)
{
    if(e->key() == Qt::Key_Slash)
    {
        return new EventSlash();
    }
    if(e->key() == Qt::Key_Asterisk)
    {
        return new EventAsterisk();
    }
    return new EventDefault();
}

Event *EventBuilder::getEvent(QKeyEvent *e){
    if(e->modifiers() & Qt::ShiftModifier)
    {
        return getEventShiftModifier(e);
    }
    else if(e->modifiers() & Qt::ControlModifier)
    {
        return getEventControlModifier(e);
    }
    else if(e->modifiers() & Qt::KeypadModifier)
    {
        return getEventKeyboardModifier(e);
    }
    else
    {
        return getEventNoModifier(e);
    }
}

