#ifndef DOCUMENTATIONENGINE_H
#define DOCUMENTATIONENGINE_H

#include <QVector>
#include <QFileInfo>

class DocumentationEngine : public QObject
{
    Q_OBJECT
public:
    explicit DocumentationEngine(QObject *parent = nullptr);
    void searchByKeyword(const QString &keyword);
    void searchByLibraryAndFunction(const QString &include, const QString &function);

    QVector<QFileInfo> documentationFiles() const;

private:
    QVector<QFileInfo> mDocumentationFiles;
};

#endif // DOCUMENTATIONENGINE_H
