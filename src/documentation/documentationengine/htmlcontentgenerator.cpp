#include "htmlcontentgenerator.h"

#include <mustache.hpp>

#include <QUrl>
#include <QDir>
#include <QString>

#include "documentationsearch.h"
#include "constantsdocumentationl.h"


std::string HTMLContentGenerator::generate(const QVector<QFileInfo> &files)
{

    const bool showIconsColumn = true;
    kainjow::mustache::mustache tmpl{mustasheTemplate};

    kainjow::mustache::data mustashe{};
    mustashe.set("showIcons", showIconsColumn ? kainjow::mustache::data::type::bool_true
                                              : kainjow::mustache::data::type::bool_false);

    kainjow::mustache::data iconsListMustashe{kainjow::mustache::data::type::list};
    kainjow::mustache::data fileNamesListMustashe{kainjow::mustache::data::type::list};

    kainjow::mustache::data dirEntryListMustashe{kainjow::mustache::data::type::list};
    for (int fileIndex = 0; fileIndex < files.size();  fileIndex++)
    {
        kainjow::mustache::data dirEntryMustashe;

        dirEntryMustashe.set("showIcons", showIconsColumn ? kainjow::mustache::data::type::bool_true
                                                          : kainjow::mustache::data::type::bool_false);
        // File Name
        QString filename = files.at(fileIndex).dir().dirName()+ "/" + files.at(fileIndex).fileName();
        dirEntryMustashe.set("fileName", filename.toStdString());
        QUrl url{DocumentationSearch::getUrl(files.at(fileIndex))};
        dirEntryMustashe.set("fileIndex", url.toString().toStdString());
        dirEntryMustashe.set("search", files.at(fileIndex).baseName().toStdString());

        dirEntryListMustashe << dirEntryMustashe;
    }

    mustashe.set("dirEntryList", dirEntryListMustashe);

    return tmpl.render(mustashe);
}
