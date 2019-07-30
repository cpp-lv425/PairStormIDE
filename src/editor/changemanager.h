#ifndef CHANGESMANAGER_H
#define CHANGESMANAGER_H

#define CHANGES_HISTORY_MAX_SIZE 200

#include<string>
#include<deque>
#include<iostream>
#include<algorithm>
#include<QString>

struct IntegralChange
{
    size_t begin_change_pos;// position where changed started
    std::string before;// string with prev delta
    std::string after;//string with current delta
};

class ChangeManager
{
public:
    std::deque<IntegralChange> changesHistory;
    std::string currentFileState;
    std::deque<IntegralChange>::iterator currentState_it;

    void limitCheck();

    void removeHistory();

    bool fileChanged(const std::string &newFileState);

public:
    ChangeManager();

    ChangeManager(const std::string &fileState);

    ~ChangeManager();

    void writeChange(std::string newFileState);

    std::string undo();

    std::string redo();
};

#endif // CHANGESMANAGER_H
