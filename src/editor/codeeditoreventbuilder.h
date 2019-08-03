#ifndef CODEEDITOREVENTBUILDER_H
#define CODEEDITOREVENTBUILDER_H
#include "codeeditorevents.h"


class EventBuilder{
private:
    EventBuilder();
    static Event * getEventNoModifier(QKeyEvent *e)
    {
        if(e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return)
            return new EventEnter();
        return nullptr;
    }
    static Event * getEventShiftModifier(QKeyEvent *e)
    {
        if(e->key()==Qt::Key_BraceLeft)
             return new EventBraceLeft();
        if(e->key()==Qt::Key_BracketLeft)
            return new EventBracketLeft();
        if(e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return)
            return new EventShiftEnter();

        return nullptr;
    }
    static Event * getEventControlModifier(QKeyEvent *e)
    {
        return nullptr;
    }


public:
   static Event * getEvent(QKeyEvent *e){
       if(e->modifiers()&Qt::NoModifier)
           return getEventNoModifier(e);
       if(e->modifiers()&Qt::ShiftModifier)
           return getEventShiftModifier(e);
       if(e->modifiers()&Qt::ControlModifier)
           return getEventControlModifier(e);
      return nullptr;
    }
};
#endif // CODEEDITOREVENTBUILDER_H
