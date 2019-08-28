#include "methodspartsdefinitiongetters.h"
#include "definitionindexes.h"
#include "classgenerationliterals.h"
#include "documentmanager.h"
#include<QRegularExpression>
#include<QDebug>

QString getTextByCursor(QTextCursor cursor)
{
    cursor.select(QTextCursor::LineUnderCursor);
    return cursor.selectedText();
}

QString getClassNameForMethodDefinition(QTextCursor cursor)
{
    while (cursor.blockNumber())// if it's first code line
    {
        QString textInCursorLine = getTextByCursor(cursor);//get line string
        if (textInCursorLine.contains("class")
           && !textInCursorLine.contains("friend"))
        {
            auto matchIter =
                    QRegularExpression(classFindingRegex).globalMatch(getTextByCursor(cursor));
            auto match = matchIter.next();
            return match.captured(1);
        }
        cursor.movePosition(QTextCursor::Up);//move up
    }
    return QString();//if nothing found, return empty string
}

bool isValidMethodInitialization(QTextCursor cursor)
{
    auto matchIter =
            QRegularExpression(validFucntionDefinition).globalMatch(getTextByCursor(cursor));
    return matchIter.hasNext();
}

MethodDefinitionPattern getMethodDefinitionPattern(const QString &funcDefinition)
{
    auto matchIter = QRegularExpression(validFucntionDefinition).globalMatch(funcDefinition);
    auto match = matchIter.next();
    // 1 is dataType  2 is methodName 3 is parametrs
    return MethodDefinitionPattern {match.captured(1), match.captured(2), match.captured(3)};
}

QString createFilePath(const QString &rootPath, const QString &file)
{
    return rootPath + '/' + file;
}

QString getMethodDefinitionName(QTextCursor cursor)// return className::fucnName or funcName is class dosn't exist
{
    auto declarationParts = getMethodDefinitionPattern(getTextByCursor(cursor));
    auto className = getClassNameForMethodDefinition(cursor);
    return declarationParts.mFunctionDataType + " " + className +
            (className.isEmpty() ? QString() : "::") + declarationParts.mFucntionName;
}

bool definitionExists(const QString documentText, QTextCursor cursor)
{
    //SAVE SOMEHOW
    QString methodFullName = getMethodDefinitionName(cursor);
    auto linesStringList = documentText.split(QRegularExpression("[\n]"), QString::SkipEmptyParts);
    for (auto &line : linesStringList)
    {
        if (line.contains(methodFullName))
        {
            auto par1 = getRowParametrsInsideBrackets(
                        getParametrsFromMethodDefinition(getTextByCursor(cursor)));

            auto par2 = getRowParametrsInsideBrackets(getParametrsFromMethodDefinition(line));
        }
    }
    return true;
}

bool isFileWithExtension(const QString &fileName, const QString &extenion)
{
    QString rFileName(fileName);
    auto splitList = rFileName.split('.');
    if (splitList.size() <= 1)// if file doesn't have the extension
    {
        return false;
    }
    return splitList[1] == extenion;// if splited second part == given in the parametrextension
}

QString getParametrsFromMethodDefinition(const QString &funcDefinition)
{
    auto matchIter =  QRegularExpression(textInsideBracketsRegex).globalMatch(funcDefinition);
    auto match = matchIter.next();
    return match.hasMatch() ? match.capturedTexts()[indexOfInsideBracketsCapture] : QString();
}

QString getRowParametrsInsideBrackets(QString textInsideBrackets)
{
    return QString()
}
