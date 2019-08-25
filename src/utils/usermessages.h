#ifndef USERMESSAGES_H
#define USERMESSAGES_H

#include <QPair>
#include <QMap>

enum class UserMessages
{
    WrongDirectoryTitle,
    WrongDirectoryMsg,
    WrongFileNameTitle,
    EmptyFileNameMsg,
    InvalidFileNameMsg,
    FileAlreadyExistsTitle,
    FileAlreadyExistsMsg,
    ErrorTitle,
    CreatingFileFailureMsg,
    FileCreatedTitle,
    FileCreatedMsg,
    SelectDirectoryTitle,
    NewFileWizardMsg,
    DocumentSavedMsg,
    FileOpeningForSavingErrorMsg,
    FileOpeningErrorMsg,
    DocumentAlreadyOpenedTitle,
    DocumentAlreadyOpenedMsg,
    OpenDirectoryTitle,
    SaveTitle,
    NoFilesToSaveMsg,
    SaveAsTitle,
    PromptSaveTitle,
    SaveQuestion,
    ConnectionFailureTitle,
    ConnectionFailureMsg,
    OpenFileTitle,
    SaveAllQuestion,
    NewProjectWizardMsg,
    EmptyDirectoryNameMsg,
    InvalidDirectoryNameTitle,
    InvalidDirectoryNameMsg,
    DirectoryAlreadyExistsTitle,
    DirectoryAlreadyExistsMsg,
    CreatingDirectoryFailureMsg,
    ProjectCreatedTitle,
    ProjectCreatedMsg,
};

extern QMap<UserMessages, QString> userMessages;

#endif // USERMESSAGES_H
