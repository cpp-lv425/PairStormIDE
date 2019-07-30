#include "changemanager.h"
#include<algorithm>
#include<QString>
ChangeManager::ChangeManager() = default;
static int count = 10;
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
    writeChange("");
}

ChangeManager::~ChangeManager() = default;
void ChangeManager::writeChange(std::string newFileState)
{
   /* QString test_start = QString::fromStdString(currentFileState);
    //qDebug()<<"before = "<< test_start;
    if(!fileChanged(newFileState))
        return;

    limitCheck();
    removeHistory();

    IntegralChange change;

    //get the start of mismatch
    auto mismatch_range_begin = std::mismatch(currentFileState.begin(), currentFileState.end(),
                                                     newFileState.begin(), newFileState.end());
    std::string before;
        std::string after;

    auto mismatch_range_end =
            std::mismatch(currentFileState.rbegin(), currentFileState.rend(),
                newFileState.rbegin(), newFileState.rend());

    size_t occuranceIndex = static_cast<size_t>(std::distance(currentFileState.begin(), mismatch_range_begin.first));

    std::copy(mismatch_range_begin.first, mismatch_range_end.first.base(), std::back_inserter(before));
    std::copy(mismatch_range_begin.second, mismatch_range_end.second.base(), std::back_inserter(after));

        if ((mismatch_range_begin.first > mismatch_range_end.first.base()) || (mismatch_range_begin.second > mismatch_range_end.second.base()))
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
    qDebug()<<"before = "<<QString::fromStdString(before);
    change.after = after;
    qDebug()<<"before = "<<QString::fromStdString(after);
    change.begin_change_pos = occuranceIndex;

    change.begin_change_pos = static_cast<size_t>(std::distance(currentFileState.begin(),
                                                            mismatch_range_begin.first));

    changesHistory.push_back(change);
    currentFileState = newFileState;
    currentState_it = changesHistory.end() - 1;


   // qDebug()<<"*******************WRITE***********************************";
    //qDebug()<<"before = "<<QString::fromStdString(currentState_it->before);
   // qDebug()<<"after = "<<QString::fromStdString(currentState_it->after);
   // qDebug()<<"position = "<< currentState_it->begin_change_pos;
   // qDebug()<<"*********************************************************";
    QString end_start = QString::fromStdString(currentFileState);
   // qDebug()<<"after = "<< end_start;
   */
    if (currentFileState == newFileState)
        {
            return;
        }

        limitCheck();


        removeHistory();
        IntegralChange change;
        auto mismatch_range_begin = std::mismatch(currentFileState.begin(), currentFileState.end(),
            newFileState.begin(), newFileState.end());



        /*auto mismatch_range_end_first = currentFileState.end();
        auto mismatch_range_end_second = currentFileState.end();
        while (true)
        {
            if (mismatch_range_end_first == currentFileState.end())
            {
                mismatch_range_end_first--;
                mismatch_range_end_second--;
                continue;
            }
            if (mismatch_range_end_first <= mismatch_range_begin.first ||
                mismatch_range_end_second <= mismatch_range_begin.second)
                break;
            if (*mismatch_range_end_first != *mismatch_range_end_second)
                break;
        }*/



        /*std::string rCurrentFileState = currentFileState;
        std::string rNewFileState = newFileState;
        std::reverse(rCurrentFileState.begin(), rCurrentFileState.end());
        std::reverse(rNewFileState.begin(), rNewFileState.end());

        auto mismatch_range_rbegin = std::mismatch(rCurrentFileState.begin(), rCurrentFileState.end(),
            rNewFileState.begin(), rNewFileState.end());


        //auto end_pos_before = std::distance(mismatch_range_rbegin.first, rCurrentFileState.end());
        //auto end_pos_after = std::distance(mismatch_range_rbegin.second, rNewFileState.end());
        auto end_pos_before = std::distance(rCurrentFileState.begin(), mismatch_range_rbegin.first);
        auto end_pos_after = std::distance(rNewFileState.begin(), mismatch_range_rbegin.second);
        //if (end_pos_before != end_pos_after)
            //std::cout << "difff";

        std::string before;
        std::string after;
        if (mismatch_range_begin.first > currentFileState.end() - end_pos_before)
        {
            before = std::string(mismatch_range_begin.first, currentFileState.end());
        }
        else
        {
            before = std::string(mismatch_range_begin.first, currentFileState.end() - end_pos_before);
        }

        if (mismatch_range_begin.second > newFileState.end() - end_pos_after)
        {
            after = std::string(mismatch_range_begin.second, newFileState.end());
        }
        else
        {
            after = std::string(mismatch_range_begin.second, newFileState.end() - end_pos_after);
        }*/

        std::string before;
        std::string after;

        auto it_before_end = currentFileState.end();
        auto it_after_end = newFileState.end();

        auto mismatch_range_end =
            std::mismatch(currentFileState.rbegin(), currentFileState.rend(),
                newFileState.rbegin(), newFileState.rend());

        size_t occuranceIndex =
            static_cast<size_t>(std::distance(currentFileState.begin(), mismatch_range_begin.first));

        /*std::copy(mismatch_range_begin.first, mismatch_range_end.first.base(), std::back_inserter(before));
        std::copy(mismatch_range_begin.second, mismatch_range_end.second.base(), std::back_inserter(after));
        */
        if ((mismatch_range_begin.first > mismatch_range_end.first.base()) || (mismatch_range_begin.second > mismatch_range_end.second.base()))
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
        std::cout<<"before = "<<before;
        change.after = after;
        std::cout<<"after = "<<after;
        change.begin_change_pos = occuranceIndex;

        //change.begin_change_pos = static_cast<size_t>(std::distance(currentFileState.begin(), mismatch_range_begin.first));
        //change.begin_change_pos = static_cast<size_t>(std::distance(newFileState.begin(), mismatch_range_begin.second));

        changesHistory.push_back(change);
        currentFileState = newFileState;
        currentState_it = changesHistory.end() - 1;
        std::cout << " ADDED\n: " << currentFileState << std::endl;
}




std::string ChangeManager::undo()
{
    if (currentState_it == changesHistory.begin())
        {
            return currentFileState;
        }
        auto pos = currentState_it->begin_change_pos;
        std::string from = currentState_it->after;
        std::string to = currentState_it->before;
        currentState_it--;
        currentFileState.replace(pos, from.length(), to);
        std::cout << "\nUNDO:\n" << currentFileState;
        return currentFileState;
}

std::string ChangeManager::redo()
{
    if (currentState_it == (changesHistory.end() - 1))//if is no any record in the change history list
    {
        qDebug()<<"NOTHING TO REDO!";
        return currentFileState;
    }

    currentState_it++;

    auto pos = currentState_it->begin_change_pos;
    std::string inert = currentState_it->after;
    /*qDebug()<<"***************************REDO**************************";
    qDebug()<<"before = "<<QString::fromStdString(currentState_it->before);
    qDebug()<<"after = "<<QString::fromStdString(currentState_it->after);
    qDebug()<<"position = "<< currentState_it->begin_change_pos;
    qDebug()<<"*********************************************************";

    qDebug()<<"***********************REDO - 1********************************";
    qDebug()<<"before = "<<QString::fromStdString((currentState_it - 1)->before);
    qDebug()<<"after = "<<QString::fromStdString((currentState_it - 1)->after);
    qDebug()<<"position = "<< (currentState_it - 1)->begin_change_pos;
    qDebug()<<"*********************************************************";*/
    currentFileState.replace(currentFileState.begin() + pos, currentFileState.begin() + pos + currentState_it->before.length(),inert);
      //  qDebug()<<"distance from the start after = " << distance;
    return currentFileState;
}
