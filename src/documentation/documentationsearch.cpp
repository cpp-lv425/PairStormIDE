#include "documentationsearch.h"

#include <QDirIterator>
QVector<QFileInfo> DocumentationSearch::search(const QDir &directory, const QString &keyword)
{
    QVector<QFileInfo> rResult;
    QDirIterator it(directory);

}
