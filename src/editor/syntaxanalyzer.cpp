#include "syntaxanalyzer.h"


SyntaxAnalyzer::SyntaxAnalyzer()
{

}

bool SyntaxAnalyzer::syntaxAnalysis(QVector<Token> line)
{
    if(!isCorrectPreprocessorCommand(line))
    {
        qDebug() << "Incorrect preprocessor command!";
        return false;
    }

    return true;
}

bool SyntaxAnalyzer::isCorrectPreprocessorCommand(QVector<Token> line)
{
    if(line.takeFirst().mName == cHash)
    {
        if(!cPreprocessorCommands.contains(line.first().mName))
        {
            return false;
        }
    }

    return true;
}

