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
    while (cursor.blockNumber())
    {
        QString textInCursorLine = getTextByCursor(cursor);
        if (textInCursorLine.contains("class")
           && !textInCursorLine.contains("friend"))
        {
            return getTextByCursor(cursor).split(' ')[indexOfClassNameInLine];//rewrite to regex
        }
        cursor.movePosition(QTextCursor::Up);
    }
    return QString();
}

bool isValidMethodInitialization(QTextCursor cursor)
{
    QRegularExpressionMatchIterator matchIter = QRegularExpression(validFucntionDefinition).
            globalMatch(getTextByCursor(cursor));
    return matchIter.hasNext();//rewrite in the future. now it's just valid every time
}

MethodDefinitionPattern getMethodDefinitionPattern(const QString &difinition)
{
    QRegularExpressionMatchIterator matchIter = QRegularExpression(validFucntionDefinition).
            globalMatch(difinition);
    QRegularExpressionMatch match = matchIter.next();
    return MethodDefinitionPattern {match.captured(1), match.captured(2), match.captured(3)};
}

QString createFilePath(const QString &rootPath, const QString &file)
{
    return rootPath + '/' + file;
}

QString getMethodDefinitionName(QTextCursor cursor)
{
    auto declarationParts = getMethodDefinitionPattern(getTextByCursor(cursor));
    auto className = getClassNameForMethodDefinition(cursor);
    return declarationParts.functionDataType + " " + className +
            (className.isEmpty() ? QString() : "::") + declarationParts.fucntionName;
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
            qDebug()<<"kek";
        }
    }
    return true;
}
/*struct MethodDefinitionPattern
{
    QString functionDataType;
    QString fucntionName;
    QString functionParametrs;
};
*/

bool isFileWithExtension(const QString &fileName, const QString &extenion)
{
    QString rFileName(fileName);
    auto splitList = rFileName.split('.');
    if (splitList.size() == 1)
    {
        return false;
    }
    return splitList[1] == extenion;
}
