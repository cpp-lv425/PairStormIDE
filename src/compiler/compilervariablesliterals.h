#ifndef COMPILERVARIABLESLITERALS_H
#define COMPILERVARIABLESLITERALS_H

#include <QString>

const QString buildFolderName = "PS_BUILD";
const QString makeFileName = "MakeFile";
const QString objectiveFilePrefix = "-o";
const QString objectiveFileExtension = ".o";
const QString sourceFileExtinsion = ".cpp";
const QString executableFileExtension = ".exe";
const QString compilerType = "g++";
const QString extraFlagsForCompilerType = "CFLAGS";
const QString extraFlagsForCompilerParams = "=-c -Wall";
const QString clenPreviousObjectiveSufics = "rm -rf *o ";
const QString compilersListNamesInSettings = "cppCompilersList";

#endif // COMPILERVARIABLESLITERALS_H
