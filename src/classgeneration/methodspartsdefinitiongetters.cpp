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
           && !textInCursorLine.contains("friend"))// if we find "class", but it's just friend class declaration
        {
            //find class name by regex
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

QString getMethodDefinitionName(QTextCursor cursor)// return className::fucnName or funcName is it's fucntion
{
    auto declarationParts = getMethodDefinitionPattern(getTextByCursor(cursor));
    auto className = getClassNameForMethodDefinition(cursor);
    return declarationParts.mFunctionDataType + " " + className +
            (className.isEmpty() ? QString() : "::") + declarationParts.mFucntionName;
}

bool definitionExists(const QString sourceText, QTextCursor cursor)
{
    //cursor is in the header file
    QString methodFullName = getMethodDefinitionName(cursor);
    auto linesStringList = sourceText.split(QRegularExpression("[\n]"), QString::SkipEmptyParts);
    for (auto &line : linesStringList)
    {
        if (line.contains(methodFullName))
        {
            //get parametrs without variables' names in the header and source files
            auto headerRowParams = getRowParametrsInsideBrackets(
                        getParametrsFromMethodDefinition(getTextByCursor(cursor)));

            auto sourceRowParams = getRowParametrsInsideBrackets(
                        getParametrsFromMethodDefinition(line));

            if (headerRowParams == sourceRowParams)
            {
                return true;
            }
        }
    }
    return false;
}

bool isFileWithExtension(const QString &fileName, const QString &extenion)
{
    QString rFileName(fileName);
    auto splitList = rFileName.split('.');
    if (splitList.size() <= 1)// if file doesn't have an extension
    {
        return false;
    }
    return splitList[inderOfExtinsionCapture] == extenion;// if splited second part == given in the parametr's extension
}

QString getParametrsFromMethodDefinition(const QString &funcDefinition)
{
    auto matchIter =  QRegularExpression(textInsideBracketsRegex).globalMatch(funcDefinition);
    auto match = matchIter.next();
    return match.hasMatch() ? match.capturedTexts()[indexOfInsideBracketsCapture] : QString();
}

QString getRowParametrsInsideBrackets(QString textInsideBrackets)
{
    QString rRowParametrs;//here we will keep parametrs without variables' names
    textInsideBrackets = removeComasInsideAngleBrackets(textInsideBrackets);
    auto parametrList = textInsideBrackets.split(',', QString::SkipEmptyParts);//split parametrs
    for (auto &parametr : parametrList)
    {
        parametr = parametr.simplified();
        //find varable name
        auto matchIter =  QRegularExpression(getVariableFromParametrs).globalMatch(parametr);
        auto match = matchIter.next();
        if (match.hasMatch())
        {
            int captureStart = match.capturedStart(1);
            int captureEnd = match.capturedEnd(1);

            QString addedText = match.capturedTexts()[0];
            //create list without variable name
            addedText.replace(captureStart,
                              captureEnd - captureStart,
                              QString());
            rRowParametrs.append(addedText);
        }
        else//if here is not variable, just add full splited parametr
        {
            rRowParametrs.append(parametr);
        }
    }
    return rRowParametrs.replace(" ","");//return without spaces
}

QString removeComasInsideAngleBrackets(QString functionParametrs)//replace all comas inside angle brackets to spaces
{
   auto openBrackets = 0;//how many angelBrackets in the parametrs
   for (auto it = functionParametrs.begin(); it != functionParametrs.end(); ++it)
   {
       switch ((*it).toLatin1())
       {
       case '>':
           openBrackets++;
           break;
       case '<':
           openBrackets--;
           break;
       case ',':
           if (openBrackets)
           {
               *it = ' ';
           }
           break;
       }
   }
   return functionParametrs;
}
