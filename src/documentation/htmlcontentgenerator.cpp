#include "htmlcontentgenerator.h"

#include <mustache.hpp>

#include <QUrl>
#include <QDir>

static std::string mustasheTemplate{R"(<!DOCTYPE html>
                                    <html>
                                        <head>
                                            <title>{{search}}</title>
                                            <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/4.7.0/css/font-awesome.min.css">
                                            </head>
                                            <body>
                                                <p>&nbsp;</p>
                                                <h1>{{search}}</h1>
                                                <p>&nbsp;</p>

                                                <table style="width:100%">
                                                    <tbody>
                                                        <tr>
                                                                 {{#showIcons}}

                                                            <td>&nbsp;</td>
                                                                 {{/showIcons}}

                                                            <td>
                                                                <strong>Name</strong>
                                                            </td>
                                                                 {{#showSize}}

                                                            <td>
                                                                <strong></strong>
                                                            </td>
                                                                 {{/showSize}}
                                                                 {{#showDetails}}

                                                            <td>
                                                                <strong></strong>
                                                            </td>
                                                                 {{/showDetails}}

                                                        </tr>
                                                                 {{#dirEntryList}}

                                                        <tr>
                                                                 {{#showIcons}}

                                                            <td>{{{fileIcon}}}</td>
                                                                 {{/showIcons}}

                                                            <td>
                                                                <a href="{{fileIndex}}">{{fileName}}</a>
                                                            </td>
                                                                 {{#showSize}}

                                                            <td>{{fileSize}}</td>
                                                                 {{/showSize}}
                                                                 {{#showDetails}}

                                                            <td>{{fileDetails}}</td>
                                                                 {{/showDetails}}

                                                        </tr>
                                                                 {{/dirEntryList}}

                                                    </tbody>
                                                </table>
                                                <p>&nbsp;</p>
                                            </body>
                                        </html>
                                                                 )"};

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

        // File Icon
        if (showIconsColumn)
        {
            dirEntryMustashe.set("showIcons", kainjow::mustache::data::type::bool_true);

            if (!files.at(fileIndex).isDir())
            {
                    dirEntryMustashe.set("fileIcon", R"(<i class="fas fa-code"></i>)");

            }
            else
            {
                dirEntryMustashe.set("fileIcon", R"(<i class="fas fa-code"></i>)");
            }
        }
        else
        {
            dirEntryMustashe.set("showIcons", kainjow::mustache::data::type::bool_false);
        }

        // File Name
        QString filename = files.at(fileIndex).dir().dirName()+ "/" + files.at(fileIndex).fileName();
        dirEntryMustashe.set("fileName", filename.toStdString());
        QUrl url{QUrl::fromLocalFile(files.at(fileIndex).filePath())};
        dirEntryMustashe.set("fileIndex", url.toString().toStdString());
        dirEntryMustashe.set("search", files.at(fileIndex).baseName().toStdString());

        dirEntryListMustashe << dirEntryMustashe;
    }

    mustashe.set("dirEntryList", dirEntryListMustashe);

    return tmpl.render(mustashe);
}

