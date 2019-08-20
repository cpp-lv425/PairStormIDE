/*
 * The purpose of this class is to read data from configuration file
 * and save it in persistent platform-independent application settings.
 * Reach out these data are posible from any point in application using statements:
 * QSettings settings;
 * int i = settings.value("fieldName").toInt();
 * where <fieldName> field in configuration file.
 *
 * Also writes application version, application name, organization name
 * to QApplication settings. They are available through static fields:
 * QApplication::applicationVersion(), QApplication::applicationName(),
 * QApplication::organizationName().
*/
#ifndef STORECONF_H
#define STORECONF_H

#include <QJsonObject>
#include <QJsonDocument>
#include <QStringList>
#include <QMap>
#include <QFile>

class StoreConf
{
public:
    explicit StoreConf(QString userName = "unnamed");

    void restoreConFile();          //  restoring settings from last session
    void saveConFile();             //  saving settings at the end of current session
private:
    void getPathToConFile();        //  search configuration file. if not exist - create it with default parameters

    QString mConFile;               //  name of configuration file
    QString mPathToConFile;         //  path to configuration file
    QMap<QString, QString> mFields; //  first -> valueName, second -> valueItself
    enum class sessionMode {Start, Finish, Size};   //  Start - start session, Finish - finish session, Size - size of enumeration

    QStringList mCppExtentionsList;     //  all extentions available in C++ mode
    QStringList mCppCompilersList;      //  all compilers available in C++ mode
    QStringList mCppLibraryHeadersList; //  all included library headers in C++ mode
    QStringList mCppLibraryBinarysList; //  all included library binarys in C++ mode

    void checkConfDir(QString str = "exe");    //  check existance directory <conf>, create if not exist
                                               //  QString str = "exe" - directory in same dir as execution file by default
    void writeJson(sessionMode mode = sessionMode::Start);     //  on application start: if conf.json not exist - creates it in conf directory
                                               //  on application quit:  rewrite json with values from QSettings
    void readJson();                //  read configuration file to [loadDoc]. status write to [readStatus]
    void parseJson();
    void saveData();                // save data to QSettings file and QApplication fields
    void separateList(QString &str, QStringList &lst);// separate <str> into tokens to <lst>

    QJsonDocument mJsonDoc;
    bool mReadStatus = false;        //  status after read conf.json 
};

#endif // STORECONF_H
