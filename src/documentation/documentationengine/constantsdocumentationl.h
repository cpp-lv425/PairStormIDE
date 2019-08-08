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
                                   <style>
                                   * {
                                       text-align: center;
                                       padding-right: 25%;
                                       padding-left: 25%;
                                       font-family: "Courier New", monospace;
                                   }

                                   table {
                                       font-size: 18px;
                                       background: white;
                                       max-width: 100%;
                                       border-collapse: collapse;
                                       text-align: left;
                                   }

                                   tr:first-child {
                                       font-weight: normal;
                                       border-bottom: 2px solid gray;
                                       padding: 10px 8px;
                                       text-align: left;
                                       background-color: white;
                                   }

                                   tr:last-child {
                                       font-weight: normal;
                                       border-bottom: 2px solid gray;
                                   }

                                   tr:first-child td {
                                       font-size: 25px;
                                      color: rgba(0, 0, 0, .8);
                                      padding: 9px 8px;
                                      text-align: center;
                                   }

                                   td {
                                      color: rgba(0, 0, 0, .5);
                                      padding: 9px 8px;
                                      text-align: left;
                                   }

                                   td a {
                                       margin-left: -84px;
                                       color: rgba(0, 0, 0, .6);
                                       text-decoration: none;
                                   }

                                   td a:hover {
                                       color: rgba(0, 0, 0, .9);
                                   }
                                   </style>
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
