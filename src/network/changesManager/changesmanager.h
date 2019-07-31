#ifndef CHANGESMANAGER_H
#define CHANGESMANAGER_H

#include <QString>
#include <iterator>
#include <algorithm>
#include "networkbase.h"
#include "change.h"

class ChangesManager
{
    QVector<QString> m_currentFiles;

public:

    ChangesManager();

    bool getChange(const QString & newFile, Change & change, const SizeType fileId);

    void applyChange(QString & newFile, const Change & change, const SizeType fileId);

    void replaceFile(const QString & newFile, const SizeType fileId);

    SizeType attachFile(const QString & newFile);

    void detachFile(const SizeType fileId);
};

#endif // CHANGESMANAGER_H
