#ifndef HTMLCONTENTGENERATOR_H
#define HTMLCONTENTGENERATOR_H

#include <string>
#include <vector>
#include <QFileInfo>

class HTMLContentGenerator
{
public:
    static std::string generate(const QVector<QFileInfo> &files);
};

#endif // HTMLCONTENTGENERATOR_H
