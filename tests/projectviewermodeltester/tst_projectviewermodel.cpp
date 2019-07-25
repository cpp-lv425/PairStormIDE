#include <QtTest>
#include <QAbstractItemModelTester>
#include "projectviewermodel.h"

class ViewerModelTests: public QObject
{
    Q_OBJECT
private slots:
    void test();
};

void ViewerModelTests::test()
{

     QStringList filters;
    filters << "*.txt"<<"*.cpp"<<"*.h"<<"*.json"<<"*.c"<<"*.hpp";
    QDir dir("C:\\Users\\igord\\Desktop\\PairStormIDE");
    ProjectViewerModel* fileSystemModel = new ProjectViewerModel(dir,filters);

    QAbstractItemModelTester tester(fileSystemModel,QAbstractItemModelTester::FailureReportingMode::QtTest,this);
}


QTEST_MAIN(ViewerModelTests)
#include "tst_projectviewermodel.moc"
