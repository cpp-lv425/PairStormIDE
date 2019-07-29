#include "changemanager.h"

ChangeManager::ChangeManager() = default;

void ChangeManager::limitCheck()
{
    if(changesHistory.size() > CHANGES_HISTORY_MAX_SIZE)
        changesHistory.pop_front();
}

void ChangeManager::removeHistory()//it's not necessary to hold history if we returned to the start and didn't press ctrl + Y
{
    if(!changesHistory.size())
        return;
    auto it_victim = changesHistory.end() - 1;// move from the end of list
    while(it_victim != currentState_it)//delete all till pass iterator position(iterator isn't point to the end only if we pressed ctrl + Z)
    {
        it_victim--;
        changesHistory.pop_back();
    }
}

bool ChangeManager::fileChanged(const std::string &newFileState)
{
    return currentFileState != newFileState;
}

//we have iterator to the start of mismatch range and position of last mismatch from the end
std::string ChangeManager::createStringFromMismatchIterators(std::string::iterator mismatch_range_begin,
                                           std::string::iterator fileStateEnd, long long end_pos)
{
//we need to do this check, because it can be the situation where one string is fully substring of another
// and the end of mistatch will be after the start of mismatch and out program will crash

    if(mismatch_range_begin > fileStateEnd - end_pos)
        return std::string(mismatch_range_begin, fileStateEnd);//create the string from start mismatch pos to the end of string
    else
    {
        //if everything is ok, just create the string from range of start mismatch position and end mismatch position
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

    //get the start of mismatch
    auto mismatch_range_begin = std::mismatch(currentFileState.begin(), currentFileState.end(),
                                                     newFileState.begin(), newFileState.end());

    std::string rCurrentFileState = currentFileState;
    std::string rNewFileState = newFileState;
    std::reverse(rCurrentFileState.begin(), rCurrentFileState.end());
    std::reverse(rNewFileState.begin(), rNewFileState.end());

    //get the start of mismatch for reversed strings
    auto mismatch_range_rbegin = std::mismatch(rCurrentFileState.begin(), rCurrentFileState.end(),
                                                      rNewFileState.begin(), rNewFileState.end());


    //the positions of the end of mismatch
    auto end_pos_before = std::distance(rCurrentFileState.begin(), mismatch_range_rbegin.first);
    auto end_pos_after =  std::distance(rNewFileState.begin(), mismatch_range_rbegin.second);

    std::string before = createStringFromMismatchIterators(mismatch_range_begin.first,
                                           currentFileState.end(), end_pos_before);

    std::string after  = createStringFromMismatchIterators(mismatch_range_begin.second,
                                                newFileState.end(), end_pos_after);

    change.before = before;
    change.after = after;


    change.begin_change_pos = static_cast<size_t>(std::distance(currentFileState.begin(),
                                                            mismatch_range_begin.first));

    changesHistory.push_back(change);
    currentFileState = newFileState;
    currentState_it = changesHistory.end() - 1;// if we add new record to the history, set the iterator to the end
}

std::string ChangeManager::undo()
{
    if(currentState_it == changesHistory.begin())
    {
        qDebug()<<"NOTHING TO UNDO";
        return currentFileState;
    }
//in order to get previous position we should replace "before" with "after"
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
// if oreder to get next position we should replace "after" from current record with "after" from next record
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
