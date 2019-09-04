#ifndef CLASSGENERATOR_H
#define CLASSGENERATOR_H

#include "filemanager.h"
#include <QWidget>
#include <QTextCursor>

namespace Ui
{
    class ClassGenerator;
}

class ClassGenerator : public QWidget
{
    Q_OBJECT

public:
    explicit ClassGenerator(QString projectPath, QWidget *parent = nullptr);
    ~ClassGenerator();

    QString className() const;
    void setClassName(const QString &className);

    QString headerName() const;
    void setHeaderName(const QString &headerName);

    QString sourceCodeName() const;
    void setSourceCodeName(const QString &sourceCodeName);

    void createFiles();
signals:
    void filesWereCreated(QString, QString);

private slots:
    void on_OkButton_clicked();

private:
    void setFilesNames();
    bool isValidClassName();
    void setAllFieldsFromUi();
    QString createHeaderMacrosName();
    QString createClassBones();
    QString createHeaderText();
    QString createSourceText();

private:
    Ui::ClassGenerator *ui;
    FileManager mFileManager;
    QString mClassName;
    QString mHeaderName;
    QString mSourceCodeName;
    QString mProjectPath;
};


#endif // CLASSGENERATOR_H
