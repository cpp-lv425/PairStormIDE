#ifndef CODEEDITOREVENTBUILDER_H
#define CODEEDITOREVENTBUILDER_H
#include "keypressevents.h"

class EventBuilder
{
private:
    static Event* getEventNoModifier(QKeyEvent *e);
    static Event* getEventShiftModifier(QKeyEvent *e);
    static Event* getEventControlModifier(QKeyEvent *e);
    static Event* getEventKeyboardModifier(QKeyEvent *e);
public:
   static Event* getEvent(QKeyEvent *e);
};
#endif // CODEEDITOREVENTBUILDER_H
