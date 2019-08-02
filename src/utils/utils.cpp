#include "utils.h"

BadWidgetCast::BadWidgetCast(): QException()
{    
}

const char* BadWidgetCast::what() const
{
    return "BadWidgetCast\n";
}

IncorrectUserInput::IncorrectUserInput():  QException()
{
}

const char *IncorrectUserInput::what() const
{
    return "Incorrect user input\n";
}

FileOpeningFailure::FileOpeningFailure(): QException()
{
}

const char* FileOpeningFailure::what() const
{
    return "Unable to open file\n";
}
