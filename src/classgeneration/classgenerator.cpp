#include "classgenerator.h"
#include "ui_classgenerator.h"
#include "classgenerationliterals.h"
#include <QRegularExpression>
#include <QPlainTextEdit>
#include <QMessageBox>
#include <QDebug>

ClassGenerator::ClassGenerator(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ClassGenerator)
{
    ui->setupUi(this);
    connect(ui->InputClassName, &QLineEdit::textChanged, this, &ClassGenerator::setFilesNames);
}

ClassGenerator::~ClassGenerator()
{
    delete ui;
}

QString ClassGenerator::className() const
{
    return mClassName;
}

void ClassGenerator::setClassName(const QString &className)
{
    mClassName = className;
}

QString ClassGenerator::headerName() const
{
    return mHeaderName;
}

void ClassGenerator::setHeaderName(const QString &headerName)
{
    mHeaderName = headerName;
}

QString ClassGenerator::sourceCodeName() const
{
    return mSourceCodeName;
}

void ClassGenerator::setSourceCodeName(const QString &sourceCodeName)
{
    mSourceCodeName = sourceCodeName;
}

void ClassGenerator::setFilesNames()
{
    ui->HeaderName->setText(ui->InputClassName->text() + headerExtension);
    ui->CodeSourceName->setText(ui->InputClassName->text() + sourceExtension);
}

bool ClassGenerator::isValidClassName()
{
    QRegularExpressionMatchIterator matchIter = QRegularExpression(validClassNameRegex).
            globalMatch(ui->InputClassName->text());
    return matchIter.hasNext();
}

void ClassGenerator::setAllFieldsFromUi()
{
    setClassName(ui->InputClassName->text());
    setHeaderName(ui->HeaderName->text());
    setSourceCodeName(ui->CodeSourceName->text());
}

QString ClassGenerator::createHeaderMacrosName()
{
    return (removeExtension(mHeaderName, headerExtension.length()) + "_H").toUpper();
}

QString removeExtension(QString str, const int extensionCharactersCount)
{
    str.chop(extensionCharactersCount);
    return str;
}

QString ClassGenerator::createClassBones()
{
    return  "\nclass " + mClassName + "\n"
            + "{\n"
            + "public:\n"
            + "\t" + mClassName + "();\n"
            + "};\n";
}

QString createMethodDefinitionBones(const QString &dataType,const QString &className,
                                    const QString &methodName, const QString &methodParams)
{
    return dataType + (dataType.isEmpty() ? QString() : " ") + className + "::" +
            methodName + "(" + methodParams + ")\n" + "{\n" + "\n}";
}

QString ClassGenerator::createHeaderText()
{
    QString macrosName = createHeaderMacrosName();
    return "#ifdef " + macrosName + "\n"
            + "#define " + macrosName + "\n"
            + createClassBones()
            + "\n#endif // " + macrosName;
}

QString ClassGenerator::createSourceText()
{
    return "#include " + mHeaderName + "\n"
            + "\n" + createMethodDefinitionBones(QString(), mClassName, mClassName, QString());
}

void ClassGenerator::on_OkButton_clicked()
{
    if (!isValidClassName())
    {
        QMessageBox::warning(this, nonSuccessCreationTitle, nonSuccessCreationMessage);
        return;
    }
    setAllFieldsFromUi();

    fileManager.createFile(mHeaderName);
    fileManager.writeToFile(mHeaderName, createHeaderText());

    fileManager.createFile(mSourceCodeName);
    fileManager.writeToFile(mSourceCodeName,createSourceText());

    QMessageBox::information(this, successCreationTitle, successCreationMessage);
}

bool definitionExists(QTextCursor cursor)
{
    QString line = getTextByCursor(cursor);
    return true;//rewrite in the future. now it's just valid every time
}

QString getTextByCursor(QTextCursor cursor)
{
    cursor.select(QTextCursor::LineUnderCursor);
    return cursor.selectedText();
}

QPair<QString, QString> getMethodNameFromFullDefinition(QString definition)
{
   QStringList listSplitedByBracket = definition.split('(', QString::SkipEmptyParts);
   QStringList listSplitedBySpace = listSplitedByBracket[leftPartOfDefinitionIndex].
           split(' ', QString::SkipEmptyParts);

   return qMakePair(listSplitedBySpace[indexOfMethodTypeInLine].simplified(),
                    listSplitedBySpace[indexOfMethodNameInLine]);
}

QString getMethodParametrsFromFullDefinition(QString definition)
{
    QRegularExpressionMatchIterator matchIter = QRegularExpression(textInsideBracketsRegex).
            globalMatch(definition);
    return matchIter.hasNext() ? matchIter.next().capturedTexts()[0] : QString();
}

QString getClassNameForMethodDefinition(QTextCursor cursor)
{
    //before it we should be sure that method bone exists(type name(parametrs). in other way, don't show add definition menu)
    while (cursor.columnNumber())
    {
        QString textInCursorLine = getTextByCursor(cursor);
        if (textInCursorLine.contains("class")
           && !textInCursorLine.contains("friend"))
        {
            return getTextByCursor(cursor).split(' ')[indexOfClassNameInLine];
        }
        cursor.movePosition(QTextCursor::Up);
    }
    return QString();
}
