#ifndef CHANGESMANAGER_H
#define CHANGESMANAGER_H
#include <string>
#include <deque>
#include <iostream>
#include <algorithm>

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
public:
    ChangesManager();
    ChangesManager(const std::string &fileState);
    ~ChangesManager();
    void writeChange(std::string newFileState);
    std::string returnToPreviousState();
};

#endif // CHANGESMANAGER_H
