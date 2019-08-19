#ifndef CHANGESMANAGER_H
#define CHANGESMANAGER_H

const int CHANGES_HISTORY_MAX_SIZE = 200;

#include<string>
#include<deque>
#include<iostream>
#include<algorithm>

struct IntegralChange
{
    size_t beginChangePos;// position where changed started
    std::string before;// string with prev delta
    std::string after;//string with current delta
};

class ChangeManager
{
public:
    std::deque<IntegralChange> mChangesHistory;
    std::string mCurrentFileState;
    std::deque<IntegralChange>::iterator mCurrentFileStateIter;

    void limitCheck();
    void removeHistory();
    bool isFileChanged(const std::string &newFileState);
    int getCursorPosPrev();
    int getCursorPosNext();

public:
    ChangeManager();
    ChangeManager(const std::string &fileState);
    ~ChangeManager();
    void writeChange(std::string newFileState);
    std::string undo();
    std::string redo();
};

#endif // CHANGESMANAGER_H
