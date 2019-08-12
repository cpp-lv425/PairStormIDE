#ifndef VIEWTEXTEDIT_H
#define VIEWTEXTEDIT_H
#include<QTextDocument>

#include <QWidget>

namespace Ui {
class ViewTextEdit;
}

class ViewTextEdit : public QWidget
{
    Q_OBJECT

public:
    explicit ViewTextEdit(QWidget *parent = nullptr);
    ~ViewTextEdit();
    QString getText();
    void setText(const QString &str);
    QTextDocument* getDocument();

private:
    Ui::ViewTextEdit *ui;
    QTextDocument *document;
};

#endif // VIEWTEXTEDIT_H
