#ifndef UTILS_H
#define UTILS_H

#include <QException>

class BadWidgetCast: public QException
{
public:
    BadWidgetCast();
public:
    virtual const char* what() const;
};

class IncorrectUserInput: public QException
{
public:
    IncorrectUserInput();
public:
    virtual const char* what() const;
};

class FileOpeningFailure: public QException
{
public:
    FileOpeningFailure();
public:
    virtual const char* what() const;
};

#endif // UTILS_H
