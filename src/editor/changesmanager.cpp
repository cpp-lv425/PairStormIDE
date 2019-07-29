#include "changemanager.h"

ChangeManager::ChangeManager() = default;

void ChangeManager::limitCheck()
{
    if(changesHistory.size() > CHANGES_HISTORY_MAX_SIZE)
        changesHistory.pop_front();
}

void ChangeManager::removeHistory()
{
    if(!changesHistory.size())
        return;
    auto it_victim = changesHistory.end() - 1;// move from the end of list
    while(it_victim != currentState_it)
    {
        it_victim--;
        changesHistory.pop_back();
    }
}

bool ChangeManager::fileChanged(const std::string &newFileState)
{
    return currentFileState != newFileState;
}

std::string ChangeManager::setStringFromMismatchIterators(std::string::iterator mismatch_range_begin,
                                           std::string::iterator fileStateEnd, long long end_pos)
{
    if(mismatch_range_begin > fileStateEnd - end_pos)
        return std::string(mismatch_range_begin, fileStateEnd);
    else
    {
        return std::string(mismatch_range_begin, fileStateEnd - end_pos);
    }
}

ChangeManager::ChangeManager(const std::string &fileState)
{
    currentFileState = fileState;
}

ChangeManager::~ChangeManager() = default;

void ChangeManager::writeChange(std::string newFileState)
{
    if(!fileChanged(newFileState))
        return;

    limitCheck();
    removeHistory();

    IntegralChange change;

    auto mismatch_range_begin = std::mismatch(currentFileState.begin(), currentFileState.end(),
                                                     newFileState.begin(), newFileState.end());

    std::string rCurrentFileState = currentFileState;
    std::string rNewFileState = newFileState;
    std::reverse(rCurrentFileState.begin(), rCurrentFileState.end());
    std::reverse(rNewFileState.begin(), rNewFileState.end());

    auto mismatch_range_rbegin = std::mismatch(rCurrentFileState.begin(), rCurrentFileState.end(),
                                                      rNewFileState.begin(), rNewFileState.end());


    auto end_pos_before = std::distance(rCurrentFileState.begin(), mismatch_range_rbegin.first);
    auto end_pos_after =  std::distance(rNewFileState.begin(), mismatch_range_rbegin.second);


    std::string before = setStringFromMismatchIterators(mismatch_range_begin.first,
                                           currentFileState.end(), end_pos_before);

    std::string after  = setStringFromMismatchIterators(mismatch_range_begin.second,
                                                newFileState.end(), end_pos_after);

    change.before = before;
    change.after = after;

    change.begin_change_pos = static_cast<size_t>(std::distance(currentFileState.begin(),
                                                            mismatch_range_begin.first));

    changesHistory.push_back(change);
    currentFileState = newFileState;
    currentState_it = changesHistory.end() - 1;
}


std::string ChangeManager::undo()
{
    if(currentState_it == changesHistory.begin())
    {
        qDebug()<<"NOTHING TO UNDO";
        return currentFileState;
    }
    auto pos = currentState_it->begin_change_pos;
    std::string from = currentState_it->after;
    std::string to = currentState_it->before;
    currentState_it--;
    currentFileState.replace(pos, from.length(), to);
    return currentFileState;
}

std::string ChangeManager::redo()
{
    if (currentState_it == (changesHistory.end() - 1))//if is no any record in the change history list
    {
        qDebug()<<"NOTHING TO REDO!";
        return currentFileState;
    }
    std::string from = currentState_it->after;
    currentState_it++;
    auto pos_start = currentState_it->begin_change_pos;
    std::string to = currentState_it->after;
    auto pos_end = pos_start + currentState_it->before.length();
    currentFileState.replace(currentFileState.begin() + static_cast<int>(pos_start),
                             currentFileState.begin() + static_cast<int>(pos_end), to);

    std::cout << "\nREDO: " << currentFileState;
    return currentFileState;
}
