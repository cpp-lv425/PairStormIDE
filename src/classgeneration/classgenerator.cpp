#include "classgenerator.h"
#include "ui_classgenerator.h"
#include "classgenerationliterals.h"
#include "filemanager.h"
#include <QRegularExpression>
#include <QPlainTextEdit>
#include <QMessageBox>


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

QString ClassGenerator::removeExtension(QString str, const int extensionCharactersCount)
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

QString ClassGenerator::createMethodDefinitionBones(const QString methodName, const QString methodParams)
{
    return mClassName + "::" + methodName + "(" + methodParams + ")\n" + "{\n" + "\n}";
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
            + "\n" + createMethodDefinitionBones(mClassName, "");
}

void ClassGenerator::on_OkButton_clicked()
{
    if (!isValidClassName())
    {
        QMessageBox::warning(this, nonSuccessCreationTitle, nonSuccessCreationMessage);
    }
    setAllFieldsFromUi();

    FileManager fileManager;

    fileManager.createFile(mHeaderName);
    fileManager.writeToFile(mHeaderName, createHeaderText());

    fileManager.createFile(mSourceCodeName);
    fileManager.writeToFile(mSourceCodeName,createSourceText());

    QMessageBox::information(this, successCreationTitle, successCreationMessage);
}
