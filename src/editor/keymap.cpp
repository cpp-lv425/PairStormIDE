#include "keymap.h"

KeyMap::KeyMap()
{
fillMap();
}

static QPair<int, QFlags<Qt::KeyboardModifier>> pairBracketLeft =qMakePair(Qt::Key_BracketLeft,Qt::NoModifier);
static void (CodeEditor::*ptreventBracketLeft)(QKeyEvent*)=&CodeEditor::eventBracketLeft;

static QPair<int, QFlags<Qt::KeyboardModifier>> pairBraceLeft = qMakePair(static_cast<int>(Qt::Key_BraceLeft),Qt::ShiftModifier);
static void (CodeEditor::*ptreventBracetLeft)(QKeyEvent*)=&CodeEditor::eventBraceLeft;

static QPair<int, QFlags<Qt::KeyboardModifier>> pairSlash = qMakePair(static_cast<int>(Qt::Key_Slash),Qt::NoModifier);
static void (CodeEditor::*ptreventSlash)(QKeyEvent*)=&CodeEditor::eventSlash;

static QPair<int, QFlags<Qt::KeyboardModifier>> pairAsterisk = qMakePair(static_cast<int>(Qt::Key_Asterisk),Qt::ShiftModifier);
static void (CodeEditor::*ptreventAsterisk)(QKeyEvent*)=&CodeEditor::eventAsterisk;

static QPair<int, QFlags<Qt::KeyboardModifier>> pairShiftEnter = qMakePair(static_cast<int>(Qt::Key_Enter),Qt::ShiftModifier);
static QPair<int, QFlags<Qt::KeyboardModifier>> pairReturnEnter = qMakePair(static_cast<int>(Qt::Key_Return),Qt::ShiftModifier);
static void (CodeEditor::*ptreventShiftEnter)(QKeyEvent*)=&CodeEditor::eventShiftEnter;



void KeyMap::fillMap(){
    mKeysMap.insert(pairBracketLeft,ptreventBracketLeft);
    mKeysMap.insert(pairBraceLeft,ptreventBracetLeft);
    mKeysMap.insert(pairSlash,ptreventSlash);
    mKeysMap.insert(pairAsterisk,ptreventAsterisk);
    mKeysMap.insert(pairShiftEnter,ptreventShiftEnter);
    mKeysMap.insert(pairReturnEnter,ptreventShiftEnter);
}

