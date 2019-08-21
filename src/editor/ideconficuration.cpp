#include "ideconfiguration.h"

void ConfigParams::setConfigParams(const QString fontStyle, const QString fontSize, const QString ideType)
{
   setFontSize(fontSize);
   setFontStyle(fontStyle);
   setIdeType(ideType);
}

int ConfigParams::getFontSize() const
{
    return mFontSize;
}

void ConfigParams::setFontSize(const QString fontSize)
{
    mFontSize = fontSize.toInt();
}

void ConfigParams::setIdeType(const QString &ideType)
{
    textColors.mBasicLiteralsColor =  QColor("#0080FF");// blue
    textColors.mCommentColor =        QColor("#009900");//green
    textColors.mStringsColor =        QColor("#CD9D2C");//orange

    if (ideType == "WHITE" || ideType == "BLUE")
    {
        textColors.mCodeTextColor =        QColor("#000000");//black
        textColors.mLineCounterAreaColor = QColor("#646060");//gray
    }
    else//dark
    {
        textColors.mCodeTextColor =        QColor("#FAF8F2");//almost white
        textColors.mLineCounterAreaColor = QColor("#202020");//gray
    }
}

QString ConfigParams::getFontStyle() const
{
    return mFontStyle;
}

void ConfigParams::setFontStyle(const QString &fontStyle)
{
    mFontStyle = fontStyle;
}
