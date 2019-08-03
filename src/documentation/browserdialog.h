#ifndef BROWSERDIALOG_H
#define BROWSERDIALOG_H

#include <QDialog>

namespace Ui {
class BrowserDialog;
}

QT_BEGIN_NAMESPACE
class MDIArea;
class ConnectionManager;
class DocumentationEngine;
QT_END_NAMESPACE

class BrowserDialog : public QDialog
{
    Q_OBJECT
public:
    explicit BrowserDialog(QWidget *parent = nullptr);
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
