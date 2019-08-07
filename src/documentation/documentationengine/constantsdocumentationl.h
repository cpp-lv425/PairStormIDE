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
                                                            <td>
                                                                <strong>Name</strong>
                                                            </td>

                                                        </tr>
                                                        {{#dirEntryList}}
                                                        <tr>
                                                            <td>
                                                                <a href="{{fileIndex}}">{{fileName}}</a>
                                                            </td>
                                                        </tr>
                                                        {{/dirEntryList}}

                                                    </tbody>
                                                </table>
                                                <p>&nbsp;</p>
                                            </body>
                                        </html>
                                                                 )"};
#endif // ULRGLOBAL_H
