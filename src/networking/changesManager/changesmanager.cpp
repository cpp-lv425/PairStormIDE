#include "changesmanager.h"

ChangesManager::ChangesManager()
{

}

bool ChangesManager::getChange(const QString & newFile, Change & change, const SizeType fileId)
{
    QString currentFile = m_currentFiles.at(static_cast<int>(fileId));
    // Find first & last mismatched element iterators
    const auto firstMismatchedElement =
            std::mismatch(currentFile.cbegin(), currentFile.cend(),
                          newFile.cbegin());

    if (firstMismatchedElement.first == currentFile.cend())
    {
        // Return if there is no changes
        change.m_index  = 0;
        change.m_before = "";
        change.m_after  = "";
        change.m_fileId = fileId;
        return false;
    }

    const auto lastMismatchedElement =
            std::mismatch(currentFile.crbegin(), currentFile.crend(),
                          newFile.crbegin());
    // Index of where has difference occured
    SizeType occurenceIndex =
            static_cast<SizeType>(std::distance(currentFile.cbegin(),
                                                firstMismatchedElement.first));
    // Two substrings that are different
    QString substringBefore;
    std::copy(firstMismatchedElement.first, lastMismatchedElement.first.base(), std::back_inserter(substringBefore));
    QString substringAfter;
    std::copy(firstMismatchedElement.second, lastMismatchedElement.second.base(), std::back_inserter(substringAfter));

    change.m_index  = occurenceIndex;
    change.m_before = substringBefore;
    change.m_after  = substringAfter;
    change.m_fileId = fileId;
    m_currentFiles[static_cast<int>(fileId)] = newFile;
    return true;
}

void ChangesManager::applyChange(QString & newFile, const Change & change, const SizeType fileId)
{
    if (static_cast<int>(fileId) < m_currentFiles.size()) return;
    QString currentFile = m_currentFiles[static_cast<int>(fileId)];
    currentFile.replace(static_cast<int>(change.m_index), change.m_before.length(), change.m_after);
    //std::replace(currentFile.begin() + change.m_index, currentFile.end(),
    //             change.m_before,
    //             change.m_after); // TODO correct QString to QChar incompatibility
    newFile = currentFile;
}

void ChangesManager::replaceFile(const QString & newFile, const SizeType fileId)
{
    if (static_cast<int>(fileId) < m_currentFiles.size()) return;
    m_currentFiles[static_cast<int>(fileId)] = newFile;
}

SizeType ChangesManager::attachFile(const QString & newFile)
{
    SizeType fileId = static_cast<SizeType>(m_currentFiles.size());
    m_currentFiles.push_back(newFile);
    return fileId;
}

void ChangesManager::detachFile(const SizeType fileId)
{
    if (static_cast<int>(fileId) < m_currentFiles.size()) return;
    m_currentFiles.erase(m_currentFiles.begin() + fileId);
}
