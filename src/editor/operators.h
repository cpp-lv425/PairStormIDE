#ifndef OPERATORS_H
#define OPERATORS_H

#include <QSet>

const QSet<QString> cOperators = {"=", "+", "-", "*", "/", "%",
                                 "++", "--", "==", "!=", ">", "<",
                                 ">=", "<=", "!", "&&", "||", "~",
                                 "&", "|", "^", "<<", ">>", "+=",
                                 "-=", "*=", "/=", "%=", "&=", "|=",
                                 "^=", "<<=", ">>=", "[", "]", "*",
                                 "&", "->", ".", "->*", ".*", "(", "#",
                                 ")", ",", "?", ":", "::", "{", "}", ";"};

const QString cBeginCommentBlock = "/*";
const QChar cDot = '.';

#endif // OPERATORS_H
