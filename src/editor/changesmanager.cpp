#include "changemanager.h"
#include<QDebug>
ChangeManager::ChangeManager() = default;

ChangeManager::ChangeManager(const std::string &fileState)
{
    currentFileState = fileState;
}

ChangeManager::~ChangeManager() = default;

void ChangeManager::limitCheck()
{
    if(changesHistory.size() > CHANGES_HISTORY_MAX_SIZE)
        changesHistory.pop_front();
}

void ChangeManager::removeHistory()//it's not necessary to hold history if we returned to the start and didn't press ctrl + Y
{
    if(!changesHistory.size())
        return;
   //delete all till pass iterator position(iterator isn't point to the end only if we pressed ctrl + Z)
    changesHistory.erase(std::next(currentState_it, 1), changesHistory.end());
}

bool ChangeManager::fileChanged(const std::string &newFileState)
{
    return currentFileState != newFileState;
}

int ChangeManager::getCursorPosNext()
{
    auto temp = currentState_it;
    if(temp == changesHistory.end() - 1)
    {
        return static_cast<int>(temp->begin_change_pos);
        //return static_cast<int>((temp - 1)->begin_change_pos);
    }
        temp++;
    return static_cast<int>(temp->begin_change_pos);
//    if(temp == changesHistory.begin())
    //temp++;
      //  return static_cast<int>(temp->begin_change_pos);

}

int ChangeManager::getCursorPosPrev()
{
    auto temp = currentState_it;
    temp++;
    if(temp == changesHistory.end())
        temp--;
    return static_cast<int>(temp->begin_change_pos);
}

void ChangeManager::writeChange(std::string newFileState)
{
    if (currentFileState == newFileState)
        return;

    limitCheck();
    removeHistory();

    IntegralChange change;
    auto mismatch_range_begin = std::mismatch(currentFileState.begin(), currentFileState.end(),
                                              newFileState.begin(), newFileState.end());

    std::string before;
    std::string after;

    auto mismatch_range_end = std::mismatch(currentFileState.rbegin(), currentFileState.rend(),
                                            newFileState.rbegin(), newFileState.rend());

    size_t occuranceIndex =
    static_cast<size_t>(std::distance(currentFileState.begin(), mismatch_range_begin.first));

//is one string is fully substring of another and they're like "0XX" and "11XX"
// here begin mismatch for "11XX" will be "XX" and end mismatch will be "1XX"
// so, we see that the end itarator has lesser adress than begin iterator
// in this case atemmpting to create a string from the range of these iterators will crash the program
// we need to check it before create the filnal string from the range of begin and end iterators
    if ((mismatch_range_begin.first > mismatch_range_end.first.base())
       || (mismatch_range_begin.second > mismatch_range_end.second.base()))
    {
        std::copy(mismatch_range_begin.first, currentFileState.end(), std::back_inserter(before));
        std::copy(mismatch_range_begin.second, newFileState.end(), std::back_inserter(after));
    }
    else
    {
        std::copy(mismatch_range_begin.first, mismatch_range_end.first.base(), std::back_inserter(before));
        std::copy(mismatch_range_begin.second, mismatch_range_end.second.base(), std::back_inserter(after));
    }

    change.before = before;
    change.after = after;
    change.begin_change_pos = occuranceIndex;
    changesHistory.push_back(change);
    currentFileState = newFileState;
    currentState_it = changesHistory.end() - 1;
}

std::string ChangeManager::undo()
{
    if (currentState_it == changesHistory.begin())
        return currentFileState;

    auto pos = currentState_it->begin_change_pos;
    std::string from = currentState_it->after;
    std::string to = currentState_it->before;

    currentState_it--;

    currentFileState.replace(pos, from.length(), to);

    return currentFileState;
}

std::string ChangeManager::redo()
{
    if (currentState_it == (changesHistory.end() - 1) ||
        currentState_it == (changesHistory.end()))//if is no any record in the change history list
        return currentFileState;

    currentState_it++;

    auto pos = currentState_it->begin_change_pos;
    std::string to   = currentState_it->after;
    std::string from = currentState_it->before;

    auto begin_replace_pos = currentFileState.begin() + static_cast<long long>(pos);
    auto end_replace_pos   = begin_replace_pos + static_cast<long long>(from.length());

    currentFileState.replace(begin_replace_pos, end_replace_pos, to);

    return currentFileState;
}
