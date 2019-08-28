#ifndef SPACES_H
#define SPACES_H

#include <QSet>

const QChar cNextLine = '\n';
const QSet<QChar> cSpaces = {'\n', ' ', '\r', '\t', '\0', '\v', QChar::ParagraphSeparator};

#endif // SPACES_H
