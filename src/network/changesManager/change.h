#ifndef CHANGE_H
#define CHANGE_H

#include "networkbase.h"

struct Change {
    SizeType    m_fileId;
    SizeType    m_index;
    QString     m_before;
    QString     m_after;
};

#endif // CHANGE_H
