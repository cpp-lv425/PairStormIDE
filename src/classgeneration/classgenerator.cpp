#include "classgenerator.h"
#include "ui_classgenerator.h"
#include "classgenerationliterals.h"

ClassGenerator::ClassGenerator(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ClassGenerator)
{
    ui->setupUi(this);
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

void ClassGenerator::setTextToOtherLineEdits()
{
    ui->HeaderName->setText(ui->InputClassName->text() + ".h");
    ui->HeaderName->setText(ui->InputClassName->text() + ".cpp");
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
    return (removeExtension(mHeaderName, 2) + "_H").toUpper();
}

QString ClassGenerator::removeExtension(QString str, const int extensionCharacterCount)
{
    str.chop(extensionCharacterCount);
    return str;
}

QString ClassGenerator::createClassBones()
{
    return (removeExtension(mHeaderName, 2) + "_H").toUpper();
}

QString ClassGenerator::createMethodDefinitionBones(const QString methodName, const QString methodParams)
{
    return mClassName + "::" + methodName + "(" + methodParams + ")\n" + "{\n" + "\n}";
}

QString ClassGenerator::createHeaderText()
{
    QString macrosName = createHeaderMacrosName();
    return "ifdef " + macrosName + "\n"
            + "define " + macrosName + "\n"
            + "\nclass " + mClassName + "\n"
            + "{\n"
            + "public:\n"
            + "\t" + mClassName + "();\n"
            + "};\n"
            + "\n#endif // " + macrosName;
}

QString ClassGenerator::createSourceText()
{
    return "#include \"" + mHeaderName + "\""
            + createMethodDefinitionBones(mClassName, "");
}

void ClassGenerator::on_OkButton_clicked()
{

}
