#ifndef UTILS_H
#define UTILS_H

#include <QException>

class BadWidgetCast: public QException
{
public:
    BadWidgetCast();
};

class IncorrectUserInput: public QException
{
public:
    IncorrectUserInput();
};

class FileOpeningFailure: public QException
{
public:
    FileOpeningFailure();
};

class DocumentPlacementFailure: public QException
{
public:
    DocumentPlacementFailure();
};

extern const char* invalidFileOrDirNameRegex;

#endif // UTILS_H
