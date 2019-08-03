#include "eventbuilder.h"

Event *EventBuilder::getEventNoModifier(QKeyEvent *e)
{
    switch(e->key())
    {
    case Qt::Key_BracketLeft:
        return new EventBracketLeft();
    case (Qt::Key_Enter | Qt::Key_Return):
        return new EventEnter();
    case Qt::Key_Apostrophe:
        return new EventApostrophe();
    case Qt::Key_Slash:
        return new EventSlash();
    default:
        return new EventDefault();
    }
}

Event *EventBuilder::getEventShiftModifier(QKeyEvent *e)
{
    switch(e->key())
    {
    case Qt::Key_BraceLeft:
        return new EventBraceLeft();
    case Qt::Key_ParenLeft:
        return new EventParenLeft();
    case Qt::Key_QuoteDbl:
        return new EventQouteDbl();
    case Qt::Key_Asterisk:
        return new EventAsterisk();
    case (Qt::Key_Enter | Qt::Key_Return):
        return new EventShiftEnter();
    case (Qt::Key_Space | Qt::Key_Return):
        return new EventSaveChangeInHistory();
    default:
        return new EventDefault();
    }
}

Event* EventBuilder::getEventControlModifier(QKeyEvent *e)
{
    switch(e->key())
    {
    case Qt::Key_Plus:
        return new EventCtrlPlus();
    case Qt::Key_Minus:
        return new EventCtrlMinus();
    case Qt::Key_Z:
        return new EventCtrlZ();
    case Qt::Key_Y:
        return new EventCtrlY();
    case Qt::Key_Slash:
        return new EventCtrlSlash();
    case Qt::Key_D:
        return new EventSendLexem();
    case Qt::Key_V:
        return new EventSaveChangeInHistory();
    default:
        return new EventDefault();
    }
}
Event *EventBuilder::getEventKeyboardModifier(QKeyEvent *e)
{
    switch(e->key())
    {
    case Qt::Key_Slash:
        return new EventSlash();
    case Qt::Key_Asterisk:
        return new EventAsterisk();
    default:
        return new EventDefault();
    }
}

Event *EventBuilder::getEvent(QKeyEvent *e)
{
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

