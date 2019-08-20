#ifndef BROWSERDIALOG_H
#define BROWSERDIALOG_H

#include <QDialog>

namespace Ui {
class BrowserDialog;
}

QT_BEGIN_NAMESPACE
class DocumentManager;
class ConnectionManager;
class DocumentationEngine;
QT_END_NAMESPACE

class BrowserDialog : public QDialog
{
    Q_OBJECT
public:
    explicit BrowserDialog(QWidget *parent = nullptr);

    BrowserDialog(const BrowserDialog&) = delete;
    BrowserDialog(BrowserDialog&&) = delete;
    BrowserDialog& operator=(const BrowserDialog&) = delete;
    BrowserDialog& operator=(BrowserDialog&&) = delete;

    ~BrowserDialog();
public slots:
    void createNewTab(const QString &keyword);
    void createEmptyTab();
private:
    ConnectionManager *mConnectionManager;
    QVector<QString> mTempHTMLFiles;
    Ui::BrowserDialog *ui;
};

#endif // BROWSERDIALOG_H
