#include "compilervariablesliterals.h"
#include "compilercontroler.h"
#include <QDirIterator>
#include "filemanager.h"
#include <QPlainTextEdit>
#include <consolewindow/consolewindow.h>
#include <QString>
#include <QProcess>
#include <QSettings>

CompilerControler::CompilerControler()
{
    mpConsoleProvider = new ConsoleServiceProvider;
}

QString CompilerControler::compilerPath() const
{
    return mCompilerPath;
}

void CompilerControler::setCompilerPath(QString compilerPath)
{
    mCompilerPath = compilerPath;
}

ConsoleServiceProvider *CompilerControler::getConsoleProvider() const
{
    return mpConsoleProvider;
}

void CompilerControler::setConsoleProvider(ConsoleServiceProvider *value)
{
    mpConsoleProvider = value;
}

QString CompilerControler::getCurrentCompilerPath() const
{
    QString rCurrentCompiler = QString();
    QSettings settings;
    if (settings.contains(compilersListNamesInSettings))
    {
        QStringList currentCompilers = settings.value(compilersListNamesInSettings).toStringList();
        if (!currentCompilers.empty())
        {
            QString firstCompilerPath = *currentCompilers.begin();
            rCurrentCompiler = firstCompilerPath;
        }
    }
    return rCurrentCompiler;
}

QString CompilerControler::getProjectPath() const
{
    return mProjectPath;
}

void CompilerControler::setProjectPath(const QString &projectPath)
{
    mProjectPath = projectPath;
}

void CompilerControler::runCompilation()
{

    auto currentCompilerName = getCurrentCompilerPath();
    sourceFilesPathes.clear();
    removeAllExecutableAndObjectsFiles();

    auto buildDirectoryPath = mProjectPath + "/" + buildFolderName;
    QDir buildDirectory(buildDirectoryPath);
    if (!buildDirectory.exists())
    {
        buildDirectory.mkdir(buildDirectoryPath);
    }

    auto makeFilePath = buildDirectoryPath + "/" + makeFileName;

    getAllSourceFilesFromTheProjectDirectory();

    FileManager fileManager;
    fileManager.createFile(makeFilePath);
    fileManager.writeToFile(makeFilePath, createMakeFileContent(getExecutibleFileName()));

    mpConsoleProvider->setWorkingDirectory(buildDirectoryPath);
    mpConsoleProvider->runConsoleCommand(currentCompilerName.append(" -f MakeFile"));
}

QString CompilerControler::getExecutibleFileName() const
{
    auto fullProjectPathParts = mProjectPath.split('/');
    return fullProjectPathParts.last();
}

void CompilerControler::removeAllExecutableAndObjectsFiles()
{
    // we should remove all files which will be created during the next compilation
    QDir dir(mProjectPath);
    dir.setNameFilters(QStringList() << "*" + objectiveFileExtension
                       << "*" + executableFileExtension
                       << makeFileName);
    dir.setFilter(QDir::Files);
    foreach (QString dirFile, dir.entryList())
    {
        dir.remove(dirFile);
    }
}

QString CompilerControler::createMakeFileContent(const QString &executibleFileName) const
{
    QString rMakeFileContent;
    rMakeFileContent +=
            "CC=" + compilerType + "\n\n" +
            extraFlagsForCompilerType + extraFlagsForCompilerParams + "\n\n" +
            "all: " + executibleFileName + "\n\n" +
            executibleFileName + ": ";

    for (auto sourceFile: sourceFilesPathes)
    {
        rMakeFileContent += sourceFile + objectiveFileExtension + " ";
    }
    rMakeFileContent += "\n\t$(CC) ";
    for (auto sourceFile: sourceFilesPathes)
    {
        rMakeFileContent += sourceFile + objectiveFileExtension + " ";
    }
    rMakeFileContent += objectiveFilePrefix + " "+ executibleFileName + "\n\n";

    for (auto sourceFile: sourceFilesPathes)
    {
        rMakeFileContent += sourceFile + objectiveFileExtension + ": " +
                mProjectPath + "/" + sourceFile + sourceFileExtension + "\n" +
                "\t$(CC) $(CFLAGS) " + mProjectPath + "/" +
                sourceFile + sourceFileExtension + "\n\n";
    }

    rMakeFileContent += "clean:\n\t"+ cleanPreviousObjectiveSufics + executibleFileName;

    return rMakeFileContent;
}

void CompilerControler::getAllSourceFilesFromTheProjectDirectory()
{
    QDirIterator fileInDirectoryIter(mProjectPath,
                                    QStringList() << "*" + sourceFileExtension,
                                    QDir::Files,
                                    QDirIterator::Subdirectories);

    while (fileInDirectoryIter.hasNext())
    {
        QFileInfo sourceFileInfo = fileInDirectoryIter.next();
        sourceFilesPathes << sourceFileInfo.completeBaseName();
    }
}

CompilerControler::~CompilerControler() = default;
