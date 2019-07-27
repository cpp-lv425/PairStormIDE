#ifndef CHANGESMANAGER_H
#define CHANGESMANAGER_H
#include <string>
#include <deque>
#include <iostream>
#include <algorithm>
#include<QDebug>
#include<QThread>
struct IntegralChange
{
    size_t begin_change_pos;// position where changed started
    std::string   before;
    std::string   after;
};

class ChangesManager
{
public:
    std::deque<IntegralChange> changesHistory;
    std::string currentFileState;
    std::deque<IntegralChange>::iterator currentState_it;
public:
    ChangesManager();
    ChangesManager(const std::string &fileState);
    ~ChangesManager();
    void writeChange(std::string newFileState);
    std::string undo();
    std::string redo();
};

#endif // CHANGESMANAGER_H
