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

#include <QWidget>
#include <QJsonObject>
#include <QJsonDocument>

class StoreConf : public QWidget
{
    Q_OBJECT
public:
    explicit StoreConf(QWidget * parent = nullptr);
private:
    QString getPathToConFile(); // search configuration file. if not exist - create it with default parameters
    QString conFile;
    QString conFileDefault          = "conf.json";

    QString organizationName        = "Lv-425.C++";
    QString applicationVersion      = "0.1";
    QString applicationName         = "Pair Storm";
    QString analizerStyle           = "white";
    QString analizerFontSize        = "12";
    QString analizerFontName        = "Courier 10 Pitch";
    QString cppExtentions           = ".c;.cpp;.h;.hpp;.json;.txt";
    QStringList cppExtentionsList {".c", ".cpp", ".h", ".hpp", ".json", ".txt"};

    void writeJson();               //  if conf.json not exist - creates it in current directory
    void readJson();                //  read [confile] to [loadDoc]. status write to [readStatus]
    void parseJson();
    void saveData();                // save data to QSettings file and QApplication fields

    QJsonDocument loadDoc;
    bool readStatus = false;        //  status to read conf.json
    bool writeStatus = false;       //  status to write default conf.json
};

#endif // STORECONF_H
