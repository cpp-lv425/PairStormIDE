#ifndef CLASSGENERATOR_H
#define CLASSGENERATOR_H

#include <QWidget>

namespace Ui {
class ClassGenerator;
}

class ClassGenerator : public QWidget
{
    Q_OBJECT

public:
    explicit ClassGenerator(QWidget *parent = nullptr);
    ~ClassGenerator();

private:
    Ui::ClassGenerator *ui;
};

#endif // CLASSGENERATOR_H
