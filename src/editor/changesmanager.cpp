#include "changemanager.h"
#include<QDebug>
ChangeManager::ChangeManager() = default;

ChangeManager::ChangeManager(const std::string &fileState)
{
    mCurrentFileState = fileState;
}

ChangeManager::~ChangeManager() = default;

void ChangeManager::limitCheck()
{
    if (mChangesHistory.size() > CHANGES_HISTORY_MAX_SIZE)
        mChangesHistory.pop_front();
}

void ChangeManager::removeHistory()//it's not necessary to hold history if we returned to the start and didn't press ctrl + Y
{
    if (!mChangesHistory.size())
        return;
    mChangesHistory.erase(std::next(mCurrentFileStateIter, 1), mChangesHistory.end());
}

bool ChangeManager::isFileChanged(const std::string &newFileState)
{
    return mCurrentFileState != newFileState;
}

int ChangeManager::getCursorPosNext()
{
    auto tempIter = mCurrentFileStateIter;
    if (tempIter == mChangesHistory.end() - 1)
    {
        return static_cast<int>(tempIter->beginChangePos);
    }
    tempIter++;
    return static_cast<int>(tempIter->beginChangePos);
}

int ChangeManager::getCursorPosPrev()
{
    auto tempIter = mCurrentFileStateIter;
    tempIter++;
    if (tempIter == mChangesHistory.end())
        tempIter--;
    return static_cast<int>(tempIter->beginChangePos);
}

void ChangeManager::writeChange(std::string newFileState)
{
    if (mCurrentFileState == newFileState)
        return;

    limitCheck();
    removeHistory();

    IntegralChange change;
    auto mismatchRangeBegin = std::mismatch(mCurrentFileState.begin(), mCurrentFileState.end(),
                            newFileState.begin(), newFileState.end());

    std::string before;
    std::string after;

    auto mismatchRangeEnd = std::mismatch(mCurrentFileState.rbegin(), mCurrentFileState.rend(),
                          newFileState.rbegin(), newFileState.rend());

    size_t occuranceIndex =
    static_cast<size_t>(std::distance(mCurrentFileState.begin(), mismatchRangeBegin.first));

//is one string is fully substring of another and they're like "0XX" and "11XX"
// here begin mismatch for "11XX" will be "XX" and end mismatch will be "1XX"
// so, we see that the end itarator has lesser adress than begin iterator
// in this case atemmpting to create a string from the range of these iterators will crash the program
// we need to check it before create the filnal string from the range of begin and end iterators
    if ((mismatchRangeBegin.first > mismatchRangeEnd.first.base())
       || (mismatchRangeBegin.second > mismatchRangeEnd.second.base()))
    {
        std::copy(mismatchRangeBegin.first, mCurrentFileState.end(), std::back_inserter(before));
        std::copy(mismatchRangeBegin.second, newFileState.end(), std::back_inserter(after));
    }
    else
    {
        std::copy(mismatchRangeBegin.first, mismatchRangeEnd.first.base(), std::back_inserter(before));
        std::copy(mismatchRangeBegin.second, mismatchRangeEnd.second.base(), std::back_inserter(after));
    }

    change.before = before;
    change.after = after;
    change.beginChangePos = occuranceIndex;
    mChangesHistory.push_back(change);
    mCurrentFileState = newFileState;
    mCurrentFileStateIter = mChangesHistory.end() - 1;
}

std::string ChangeManager::undo()
{
    if (mCurrentFileStateIter == mChangesHistory.begin())
        return mCurrentFileState;

    std::string from = mCurrentFileStateIter->after;
    std::string to = mCurrentFileStateIter->before;

    mCurrentFileStateIter--;

    mCurrentFileState.replace(mCurrentFileStateIter->beginChangePos, from.length(), to);

    return mCurrentFileState;
}

std::string ChangeManager::redo()
{
    if (mCurrentFileStateIter == (mChangesHistory.end() - 1)
        || mCurrentFileStateIter == (mChangesHistory.end()))//if is no any record in the change history list
            return mCurrentFileState;

    mCurrentFileStateIter++;

    std::string to   = mCurrentFileStateIter->after;
    std::string from = mCurrentFileStateIter->before;

    auto begin_replace_pos = mCurrentFileState.begin() +
                           static_cast<long long>(mCurrentFileStateIter->beginChangePos);
    auto end_replace_pos   = begin_replace_pos + static_cast<long long>(from.length());

    mCurrentFileState.replace(begin_replace_pos, end_replace_pos, to);

    return mCurrentFileState;
}
