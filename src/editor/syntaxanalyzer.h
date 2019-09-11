#ifndef SYNTAXANALYZER_H
#define SYNTAXANALYZER_H

#include <QList>
#include <QVector>
#include <QDebug>
#include <QSet>
#include <QString>
#include <QChar>
#include <QDebug>
#include <token.h>

const QChar cHash = '#';
const QSet<QString> cPreprocessorCommands = {
    "include",
    "define",
};

class SyntaxAnalyzer
{
public:
    SyntaxAnalyzer();
    bool syntaxAnalysis(QVector<Token>);
    bool isCorrectPreprocessorCommand(QVector<Token>);
};

#endif // SYNTAXANALYZER_H
