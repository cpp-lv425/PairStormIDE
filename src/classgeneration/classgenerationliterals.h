#ifndef CLASSGENERATIONLITERALS_H
#define CLASSGENERATIONLITERALS_H
#include <QString>

const QString validClassNameRegex       = "^[a-zA-Z_]+([_]?[a-zA-Z0-9])*$";
const QString validFucntionDefinition   = "^(?:[ \t])*((?:[^ \t\n])+)(?:[ \t\n])+((?:[^ \t\n])+)(?:[ \t\n])*"
                                          "\\({1}((?:[^)])*)\\){1}(?:[ \t\n])*;{1}(?:[ \t])*$";
const QString textInsideBracketsRegex   = "(?<=\\().+?(?=\\))";
const QString successCreationTitle      = "Successful creation";
const QString successCreationMessage    = "The class was successfully created!";
const QString nonSuccessCreationTitle   = "Forbidden creation";
const QString nonSuccessCreationMessage = "The class name is not correct!";
const QString headerExtension           = ".h";
const QString sourceExtension           = ".cpp";

#endif // CLASSGENERATIONLITERALS_H
