#ifndef TABCPP_H
#define TABCPP_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QComboBox;
class QListWidget;
class QPushButton;
class QListWidgetItem;
QT_END_NAMESPACE

class TabCpp : public QWidget
{
    Q_OBJECT
public:
    explicit TabCpp(QWidget *parent = nullptr);

    const QString & getStandartCurrernt() const;
    const QString & getStandartNew() const;
    void setStandartCurrernt(const QString &);

private:
    // variables to handle C++ standarts
    QStringList mStandartList         {"C++98", "C++03", "C++11", "C++14", "C++17"};
    QString mStandartDefault         = "C++11";
    QString mStandartCurrernt;
    QString mStandartNew;
    QComboBox *mpComboStandart;

    // variables to handle paths to compiler
    QStringList mCompilersListDefault;
    QStringList mCompilersList;
    QListWidget *mpCompilersList;
    QPushButton *mpAddCompiler;

    // variables to handle headers includes

    // variables to handle binarys includes

    // design parameters
    int mColumnSpasing              = 10;
    int mStretchFactor              = 20;
    int mStretchFactorMain          = 100;
    int mCompilerWidth              = 100;
    int mBasicStretch               = 1;
public slots:
    void onChangeStandart(const QString&);
    void onAddCompilerClicked();
    void onCompilerListDoubleClicked(QListWidgetItem * item);
signals:
    void compilersListChanged(const QStringList);
};

#endif // TABCPP_H
