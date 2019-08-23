#ifndef CLASSGENERATIONLITERALS_H
#define CLASSGENERATIONLITERALS_H
#include <QString>

const QString validClassNameRegex       = "^[a-zA-Z_]+([_]?[a-zA-Z0-9])*$";
const QString successCreationTitle      = "Successful creation";
const QString successCreationMessage    = "The class was successfully created!";
const QString nonSuccessCreationTitle   = "Forbidden creation";
const QString nonSuccessCreationMessage = "The class name is not correct!";

#endif // CLASSGENERATIONLITERALS_H
