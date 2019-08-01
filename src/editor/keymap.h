#ifndef KEYMAP_H
#define KEYMAP_H
#include <codeeditor.h>

class KeyMap
{
public:
    KeyMap();
    //Qmap with different hot keys
    //key - is a Qpair, witch consist of int number returned by QKeyEvent::key() and QFlag returned by QKeyEvent::modifiers()
    //value - is a pointer to CodeEditor`s method, handler of keyboard event
    QMap<QPair<int, QFlags<Qt::KeyboardModifier>>, void(CodeEditor::*)(QKeyEvent *)> mKeysMap;
    void fillMap();
    decltype(auto) getkeys()
    {
          return mKeysMap;
    }
};

#endif // KEYMAP_H
