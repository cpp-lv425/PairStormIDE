#include "utils.h"

BadWidgetCast::BadWidgetCast(): QException()
{    
}

IncorrectUserInput::IncorrectUserInput():  QException()
{
}

FileOpeningFailure::FileOpeningFailure(): QException()
{
}

DocumentPlacementFailure::DocumentPlacementFailure(): QException()
{
}
