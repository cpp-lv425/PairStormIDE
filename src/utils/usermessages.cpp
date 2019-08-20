#include "usermessages.h"

QMap<UserMessages, QString> userMessages =
{
    std::pair<UserMessages, const QString>(UserMessages::WrongDirectoryTitle, "Wrong directory"),
    std::pair<UserMessages, const QString>(UserMessages::WrongDirectoryMsg, "Please specify correct file directory."),
    std::pair<UserMessages, const QString>(UserMessages::WrongFileNameTitle, "Incorrect name"),
    std::pair<UserMessages, const QString>(UserMessages::EmptyFileNameMsg, "Please enter file name."),
    std::pair<UserMessages, const QString>(UserMessages::InvalidFileNameMsg, "Invalid file name."),
    std::pair<UserMessages, const QString>(UserMessages::FileAlreadyExistsTitle, "File exists"),
    std::pair<UserMessages, const QString>(UserMessages::FileAlreadyExistsMsg, "File with specified name already exists. Please select other file name."),
    std::pair<UserMessages, const QString>(UserMessages::ErrorTitle, "Error"),
    std::pair<UserMessages, const QString>(UserMessages::CreatingFileFailureMsg, "Unable to create file"),
    std::pair<UserMessages, const QString>(UserMessages::FileCreatedTitle, "File Created"),
    std::pair<UserMessages, const QString>(UserMessages::FileCreatedMsg, "Specified file has been successfully created."),
    std::pair<UserMessages, const QString>(UserMessages::SelectDirectoryTitle, "Select project directory"),
    std::pair<UserMessages, const QString>(UserMessages::NewFileWizardMsg, "Please specify file name, file extension and project directory."),
    std::pair<UserMessages, const QString>(UserMessages::DocumentSavedMsg, "Changes to document have been saved"),
    std::pair<UserMessages, const QString>(UserMessages::FileOpeningForSavingErrorMsg, "Unable to open file for saving"),
    std::pair<UserMessages, const QString>(UserMessages::FileOpeningErrorMsg, "Unable to open specified file."),
    std::pair<UserMessages, const QString>(UserMessages::DocumentAlreadyOpenedTitle, "Document already opened"),
    std::pair<UserMessages, const QString>(UserMessages::DocumentAlreadyOpenedMsg, "Selected document is already opened."),
    std::pair<UserMessages, const QString>(UserMessages::OpenDirectoryTitle, "Open Directory"),
    std::pair<UserMessages, const QString>(UserMessages::SaveTitle, "Save"),
    std::pair<UserMessages, const QString>(UserMessages::NoFilesToSaveMsg, "There are no opened documents."),
    std::pair<UserMessages, const QString>(UserMessages::SaveAsTitle, "Save As"),
    std::pair<UserMessages, const QString>(UserMessages::PromptSaveTitle, "Save Changes"),
    std::pair<UserMessages, const QString>(UserMessages::SaveQuestion, "Do you want to save changes?"),
    std::pair<UserMessages, const QString>(UserMessages::ConnectionFailureTitle, "Launching server"),
    std::pair<UserMessages, const QString>(UserMessages::ConnectionFailureMsg, "Startup Failed"),
    std::pair<UserMessages, const QString>(UserMessages::OpenFileTitle, "Open File"),
    std::pair<UserMessages, const QString>(UserMessages::SaveAllQuestion, "The following documents have been changed. \nWould you like to save changes?"),
};
