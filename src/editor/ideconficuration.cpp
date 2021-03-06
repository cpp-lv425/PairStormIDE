#include "ideconfiguration.h"
#include <QDebug>
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
    textColors.mWaveUnderlineColor =  QColor("#FF0000");//red

    if (ideType == "WHITE" || ideType == "BLUE")
    {
        mIdeType = "WHITE";
        textColors.mCodeTextColor =        QColor("#000000");//black
        textColors.mLineCounterAreaColor = QColor("#C0C0C0");//light gray
    }
    else//dark
    {
        mIdeType = "DARK";
        textColors.mCodeTextColor =        QColor("#FAF8F2");//almost white
        textColors.mLineCounterAreaColor = QColor("#373A38");//dark gray
    }
}

QString ConfigParams::getIdeType()
{
    return mIdeType;
}

QString ConfigParams::getFontStyle() const
{
    return mFontStyle;
}

void ConfigParams::setFontStyle(const QString &fontStyle)
{
    mFontStyle = fontStyle;
}
