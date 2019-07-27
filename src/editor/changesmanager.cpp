#include "changesmanager.h"

ChangesManager::ChangesManager() = default;

ChangesManager::ChangesManager(const std::string &fileState)
{
    currentFileState = fileState;
    currentState_it = changesHistory.begin();
}

ChangesManager::~ChangesManager() = default;

void ChangesManager::writeChange(std::string newFileState)
{
    if(currentFileState == newFileState)//if nothing changed
    {
        return;
    }
    IntegralChange change;
    //finding  the first mistatch in the strings
    //pair where [first] is iterator for first mismatch in the current string
    // [second] is iterator for first mismatch in the new string
    auto mismatched_range_begin = std::mismatch(currentFileState.begin(), currentFileState.end(),
         newFileState.begin(), newFileState.end());
    qDebug()<<"11111111111111111111";
    std::string rCurrentFileState = currentFileState;
    std::string rNewFileState = newFileState;

    //get the reversed strings
    std::reverse(rCurrentFileState.begin(), rCurrentFileState.end());
    std::reverse(rNewFileState.begin(), rNewFileState.end());
    qDebug()<<"2222222222222222";
    //finding the first mistatch in the reversed strings
    auto mismatched_range_rbegin = std::mismatch(rCurrentFileState.begin(), rCurrentFileState.end(),
                                                   rNewFileState.begin(), rNewFileState.end());
    // get the positons of last mismatch. It'll be the distance between first mistmatch in the reversed strings and end of reversed strings
    // for exaple, we have strings "textTEXT" and "text00TEXT". first mismatch will be {"TEXT", "00TEXT"}
 qDebug()<<"33333333333333";
    // we reverse strings and get "TXETtxet" and "TXET00txet". first mismatch will be {"txet", "00txet"}
    // we find the distance between "txet", "00txet" and the .end() of each strings and get 4 and 6 in accordance
    // these indexes are the end of the mismatch in the not reversed strings

    // so, now we have (text[start_mismatch, end_mismatch]TEXT) for first string. both iterators point to the same position
    // and (text[start_mismatch]00[end_mismatch]TEXT)
 qDebug()<<"44444444444444";
    auto end_pos_before = std::distance(mismatched_range_rbegin.first, rCurrentFileState.end());
    auto end_position_after = std::distance(mismatched_range_rbegin.second, rNewFileState.end());

    // we have the range of mismatch and just copy it
    qDebug()<<"555555555555555";
    std::string before;
    if(currentFileState.begin() + end_pos_before < mismatched_range_begin.first)
    {
        before = std::string(mismatched_range_begin.first, currentFileState.end());
         qDebug()<<"6666666666666666666";
    }
    else
    {
        before = std::string(mismatched_range_begin.first, currentFileState.begin() + end_pos_before);// in accorfance to our example it'll be ""
    }
    change.before = before;
    qDebug()<<"777777777777777777";
    std::string after;
    if(currentFileState.begin() + end_position_after  < mismatched_range_begin.second)
    {
        after = std::string(mismatched_range_begin.second, newFileState.end());
    }
    else
    {
        after = std::string(mismatched_range_begin.second, newFileState.begin() + end_position_after);// in accorfance to our example it'll be "00"
    }
    change.after = after;
     qDebug()<<"888888888888888888888";
    change.begin_change_pos =  static_cast<size_t>(std::distance(currentFileState.begin(),
                                                   mismatched_range_begin.first));// start position of mismatch

    currentFileState = newFileState;
    changesHistory.push_back(change);
    currentState_it = changesHistory.end();//iterator points to the current position
    qDebug()<<"current Size = "<<changesHistory.size();
}

std::string ChangesManager::undo()// replace text and return previous version
{
    qDebug()<<"99999999999999999999999";
    if( currentState_it == changesHistory.begin())//if is no any record in the change history list
    {
        qDebug()<<"NOTHING TO UNDO!";
        return currentFileState;
    }
    //if(currentState_it == changesHistory.begin())//if is no any record in the change history list
       // return currentFileState;
      qDebug()<<"0000000000000000000000";
    currentState_it--;
    auto pos = currentState_it->begin_change_pos;
    std::string from = currentState_it->after;
    std::string   to = currentState_it->before;
      qDebug()<<"AAAAAAAAAAAAAAAAAAAAAAA";
    currentFileState.replace(pos,from.length(), to);
    return currentFileState;
}

std::string ChangesManager::redo()
{
    qDebug()<<"redo current size = "<<changesHistory.size();
    if(currentState_it == changesHistory.end())//if is no any record in the change history list
    {
        qDebug()<<"NOTHING TO REDO";
        return currentFileState;
    }
    auto pos = currentState_it->begin_change_pos;
    std::string from = currentState_it->before;
    std::string to   = currentState_it->after;
    currentState_it++;

    currentFileState.replace(pos,from.length(), to);
    return currentFileState;
}
