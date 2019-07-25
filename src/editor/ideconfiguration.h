#ifndef IDECONFIGURATION_H
#define IDECONFIGURATION_H
#include<QString>
#include<QColor>
#include<QPlainTextEdit>
#include <QDebug>

struct ConfigParams
{
    QColor mCodeTextColor;
    QColor mCurrentLineColor;// line where we're standing
    QColor mBasicLiteralsColor;// int, double, main, return, functions names
    QColor mUserTypesColor;
    QColor mDefineTypesColor;
    QColor mCommentColor;
    QColor mLineCounterAreaColor;
    QColor mStringsColor;

    QString mTextStyle;
    int mFontSize;

    void setConfigParams(QString textStyle, QString fontSize, QString ideType)
    {
        mTextStyle = textStyle;
        mFontSize = fontSize.toInt();
        mBasicLiteralsColor = QColor("#5377DB");// blue
        mUserTypesColor = QColor("#46C976");//light green
        mDefineTypesColor = QColor("#CA55E8");//purpule
        mCommentColor = QColor("#116390");//green
        mLineCounterAreaColor = QColor("#B5A8A8");//gray
        mStringsColor = QColor("#CD9D2C");//orange
        mCurrentLineColor = QColor(80, 20, 80);

        if(ideType == "WHITE" || ideType == "BLUE")
        {
            mCodeTextColor = QColor("#000000");//black
            mCurrentLineColor = QColor("#EBDFE8");//very light gray
        }

        else//dark
        {
            mCodeTextColor = QColor("#FAF8F2");//almost white
            mCurrentLineColor = QColor("#8010FF");// almost black
        }
    }
};
#endif // IDECONFIGURATION_H
