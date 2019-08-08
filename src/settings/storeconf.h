/*
 * The purpose of this class is to read data from configuration file
 * and save it in persistent platform-independent application settings.
 * Reach out these data are posible from any point in application using statements:
 * QSettings settings;
 * int i = settings.value("fieldName").toString();
 * where <fieldName> same as in configuration file.
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

    void restoreConFile();
    void saveConFile();
private:
    void getPathToConFile();        // search configuration file. if not exist - create it with default parameters

    QString mConFile;
    QString mPathToConFile;
    QMap<QString, QString> mFields;  //  first -> valueName, second -> valueItself

    QStringList mCppExtentionsList;

    void writeJson(QString str = "start");     //  on application start: if conf.json not exist - creates it in conf directory
                                               //  on application quit:  rewrite json from QSettings
    void readJson();                //  read [confile] to [loadDoc]. status write to [readStatus]
    void parseJson();
    void saveData();                // save data to QSettings file and QApplication fields

    QJsonDocument mJsonDoc;
    bool mReadStatus = false;        //  status to read conf.json
    bool mWriteStatus = false;       //  status to write default conf.json
};

#endif // STORECONF_H
