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

    // login validation
    // Github username may only contain alphanumeric characters or hyphens.
    // Github username cannot have multiple consecutive hyphens.
    // Github username cannot begin or end with a hyphen.
    // Maximum is 39 characters
    rx.setPattern("^(?:[\\w]{1}(?:(?:[\\w])|(?:-[^-])){6,37}[\\w]?)$");
    v.setRegExp(rx);
    pos = 0;
    s = login;
    state = v.validate(s, pos);
    return (state == QValidator::Acceptable) ? true : false;
}
