#ifndef CHANGESMANAGER_H
#define CHANGESMANAGER_H

#define CHANGES_HISTORY_MAX_SIZE 100
#include <string>
#include <deque>
#include <iostream>
#include <algorithm>
#include<QDebug>
#include<QThread>
#include<QPlainTextEdit>
#include<QCursor>
struct IntegralChange
{
    size_t begin_change_pos;// position where changed started
    std::string   before;
    std::string   after;
    QPoint cursor;
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
    std::string createStringFromMismatchIterators(std::string::iterator mismatch_range_begin,
                                               std::string::iterator fileStateEnd, long long end_pos);

public:
    ChangeManager();
    ChangeManager(const std::string &fileState);
    ~ChangeManager();
    void writeChange(std::string newFileState, QPlainTextEdit *codeEditor);
    std::string undo(QPlainTextEdit *codeEditor);
    std::string redo(QPlainTextEdit *codeEditor);
};

#endif // CHANGESMANAGER_H
