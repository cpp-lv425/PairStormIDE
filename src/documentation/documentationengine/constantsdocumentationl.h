#ifndef ULRGLOBAL_H
#define ULRGLOBAL_H

#include <QUrl>

namespace ReferenceUrl
{
    const QUrl google("http://www.google.com");
    const QUrl reference("https://en.cppreference.com/w/");
}
const std::string mustasheTemplate{R"(<!DOCTYPE html>
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
#endif // ULRGLOBAL_H
