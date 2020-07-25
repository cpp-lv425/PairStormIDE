#ifndef IDECONFIGURATION_H
#define IDECONFIGURATION_H
#include<QString>
#include<QColor>
#include<QTextFormat>

struct TextColors
{
    QColor mStringsColor;
    QColor mBasicLiteralsColor;
    QColor mCommentColor;
    QColor mCodeTextColor;
    QColor mLineCounterAreaColor;
    QColor mWaveUnderlineColor;
};
struct ConfigParams
{
    TextColors textColors;
    QString mFontStyle;
    QString mIdeType;
    int mFontSize;
public:
    QString getFontStyle() const;
    void setFontStyle(const QString &fontStyle);
    int getFontSize() const;
    void setFontSize(const QString fontSize);
    QString getIdeType();
    void setIdeType(const QString &ideType);
    void setConfigParams(const QString fontStyle, const QString fontSize, const QString ideType);
};

#endif // IDECONFIGURATION_H
