#ifndef STORECONF_H
#define STORECONF_H

#include <QWidget>
#include <QJsonDocument>
#include <QJsonObject>

class StoreConf : public QWidget
{
    Q_OBJECT
public:
    explicit StoreConf(QWidget * parent = nullptr);
    ~StoreConf();
private:
    // search configuration file, asking user for path. if not exist - create it with default param
    QString getPathToConFile();
    QString conFile;
    QString conFileDefault          = "conf.json";

    QString organizationName        = "Lv-425.C++";
    QString applicationVersion      = "0.1";
    QString applicationName         = "Pair Storm";
    QString analizerStyle           = "white";
    QString analizerFontSize        = "12";
    QString analizerFontName        = "Times New Roman";
    QString cppExtentions           = "c;cpp;h;hpp;json;txt";
    QStringList cppExtentionsL;

    void writeJson();
    void readJson();                //  read [confile] to [loadDoc]. status write to [readStatus]
    void parseJson();
    void saveData();                // save data to QSettings file and QApplication fields

    QJsonDocument loadDoc;
    bool readStatus = false;        //  readStatus from conf.json
    bool writeStatus = false;       //  writeStatus writing default conf.json
};

#endif // STORECONF_H
