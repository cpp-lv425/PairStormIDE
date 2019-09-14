#ifndef VALIDATOR_H
#define VALIDATOR_H

class QString;

class Validator
{
public:
    Validator();

    static bool forGitHub(const QString &login, const QString &password);
};

#endif // VALIDATOR_H
