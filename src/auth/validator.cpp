#include "validator.h"

#include <QDebug>
#include <QRegExpValidator>

Validator::Validator()
{

}

bool Validator::forGitHub(const QString &login, const QString &password)
{
    // password validation
    QRegExp rx("\\S{8,39}");    // according to GitHub rules password length in range 8 - 39 characters
    QRegExpValidator v(rx, nullptr);
    QString s = password;
    int pos = 0;
    auto state = v.validate(s, pos);
    if (state != QValidator::Acceptable)
    {
        return false;
    }
    // [^-]\\S{2,37}[^-]
    // login validation
    rx.setPattern("^(?:[_\d\w]{1}(?:(?:[_\d\w])|(?:-[^-])){6,37}[_\d\w]{1})$");// according to GitHub rules first and last characters can't be '-'
    v.setRegExp(rx);
    pos = 0;
    s = login;
    state = v.validate(s, pos);
    return (state == QValidator::Acceptable) ? true : false;
}
