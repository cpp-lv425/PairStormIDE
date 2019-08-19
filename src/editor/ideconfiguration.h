#ifndef IDECONFIGURATION_H
#define IDECONFIGURATION_H
#include<QString>
#include<QColor>
#include<QDebug>
struct ConfigParams
{
    QColor mCodeTextColor;
    QColor mCurrentLineColor;// line where we're standing
    QColor mBasicLiteralsColor;// int, double, main, return, for, etc
    QColor mUserTypesColor;// user's classes color
    QColor mDefineTypesColor;// aliases values
    QColor mCommentColor;
    QColor mLineCounterAreaColor;//backg
    QColor mStringsColor;// text between "..."

    QString mTextStyle;
    int mFontSize;

    void setConfigParams(QString textStyle, QString fontSize, QString ideType)
    {
       mTextStyle = textStyle;
       mFontSize  = fontSize.toInt();
       mBasicLiteralsColor =   QColor("#0080FF");// blue
       mUserTypesColor =       QColor("#46C976");//light green
       mDefineTypesColor =     QColor("#CA55E8");//purpule
       mCommentColor =         QColor("#009900");//green
       mStringsColor =         QColor("#CD9D2C");//orange

       if (ideType == "WHITE" || ideType == "BLUE")
       {
           mCodeTextColor =    QColor("#000000");//black
           mLineCounterAreaColor = QColor("#B5A8A8");//gray
       }
       else//dark
       {
           mCodeTextColor =    QColor("#FAF8F2");//almost white
           mLineCounterAreaColor = QColor("#404040");//gray
       }
    }
};
#endif // IDECONFIGURATION_H
