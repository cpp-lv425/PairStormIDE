#ifndef HIGHLIGHTERCPP_H
#define HIGHLIGHTERCPP_H
#include <QSyntaxHighlighter>

#include "lexercpp.h"
class Highlightercpp: QSyntaxHighlighter
{
public:
    Highlightercpp(QTextDocument *parent);
};

#endif // HIGHLIGHTERCPP_H
