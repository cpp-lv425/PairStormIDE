/*
 * class provides services:
 *  get access to configuration file;
 *  changing application style
 */
#ifndef TABENVIRONMENT_H
#define TABENVIRONMENT_H

#include <QWidget>
#include <QtWidgets>

class TabEnvironment : public QWidget
{
    Q_OBJECT
public:
    explicit TabEnvironment(QWidget *parent = nullptr);

    const QString & getStyleCurrernt() const;
    const QString & getStyleNew() const;
    void setStyleCurrernt(const QString &);

private:
    // variables to handle Style
    QStringList mEditorStylesList         {"WHITE", "BLUE", "DARK"};
    QString mStyleDefault                = "WHITE";
    QString mStyleCurrernt;
    QString mStyleNew;
    QComboBox *mpComboEditorStyle;

    // variables to handle configuration file
    QString mCurrentPath;

    // design parameters
    int mRowSpasing              = 10;
    int mEditorStyleWidth        = 120;
    int mModifyBtnWidth          = 120;
public slots:
    void onClicked();                   //  open configuraton file in default application
    void onChangeStyle(const QString &);
};

#endif // TABENVIRONMENT_H
