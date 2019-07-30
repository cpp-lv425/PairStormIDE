#include "storeconf.h"

#include <QString>
#include <QFileDialog>
#include <QDebug>
#include <QJsonArray>
#include <QApplication>
#include <QSettings>

using std::string;

StoreConf::StoreConf(QWidget * parent)
    : QWidget (parent)
{
    conFile = getPathToConFile();
    if (!conFile.size()) // keep all fields with default values, create conf.json with these values
    {
        writeJson();
        saveData();
    } else {
        readJson();
        parseJson();
        saveData();
    }
}

StoreConf::~StoreConf() { }

QString StoreConf::getPathToConFile()
{
    QString currentPath = QDir::currentPath();
    currentPath += "/conf.json";
    QFile file(currentPath);
    if(file.exists())
        return "conf.json";

    QFileDialog fd(this);
    fd.setObjectName("fileDialog");
    QString str = fd.getOpenFileName(this, "Choose configuration file *.json", "", "*.json");
    //QString str = QFileDialog::getOpenFileName(this, "Choose configuration file *.json", "", "*.json");
    std::string utf8_text = str.toUtf8().constData();
    if (utf8_text.size() == 0) {    // user chose Cancel button.
        return "";
    }
    return str;
}

void StoreConf::writeJson()
{
    QJsonObject root_obj;
    root_obj.insert("organizationName", organizationName);
    root_obj.insert("applicationVersion", applicationVersion);
    root_obj.insert("applicationName", applicationName);
    root_obj.insert("analizerStyle", analizerStyle);
    root_obj.insert("analizerFontSize", analizerFontSize);
    root_obj.insert("analizerFontName", analizerFontName);
    root_obj.insert("cppExtentions", cppExtentions);

    QJsonDocument json_doc(root_obj);
    QString json_string = json_doc.toJson();

    //if (!conFile.size())
        conFile = conFileDefault;

    QFile save_file(conFile);
    if(!save_file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        writeStatus = false;
        return;
    } else
        writeStatus = true;

    save_file.write(json_string.toLocal8Bit());
}

void StoreConf::readJson()
{
    QFile loadFile(conFile);        // close in destructor
    if (loadFile.open(QIODevice::ReadOnly))
            readStatus = true;
    else {
        readStatus = false;
        return;
    }

    QByteArray saveData = loadFile.readAll();
    loadDoc = QJsonDocument::fromJson(saveData);
    if(loadDoc.isNull() || !loadDoc.isObject() || loadDoc.isEmpty())
        readStatus = false;
    else
        readStatus = true;
}

void StoreConf::parseJson()
{
    QJsonObject json = loadDoc.object();

    if (json.contains("organizationName") && json["organizationName"].isString())
        organizationName = json["organizationName"].toString();

    if (json.contains("applicationVersion") && json["applicationVersion"].isString())
        applicationVersion = json["applicationVersion"].toString();

    if (json.contains("applicationName") && json["applicationName"].isString())
        applicationName = json["applicationName"].toString();

    if (json.contains("analizerStyle") && json["analizerStyle"].isString())
        analizerStyle = json["analizerStyle"].toString();

    if (json.contains("analizerFontSize") && json["analizerFontSize"].isString())
        analizerFontSize = json["analizerFontSize"].toString();

    if (json.contains("analizerFontName") && json["analizerFontName"].isString())
        analizerFontName = json["analizerFontName"].toString();

    if (json.contains("cppExtentions") && json["cppExtentions"].isString())
        cppExtentions = json["cppExtentions"].toString();
}

void StoreConf::saveData()
{
    QSettings settings(organizationName, applicationName);
    settings.setValue("organizationName", organizationName);
    settings.setValue("applicationVersion", applicationVersion);
    settings.setValue("applicationName", applicationName);
    settings.setValue("analizerStyle", analizerStyle);
    settings.setValue("analizerFontSize", analizerFontSize);
    settings.setValue("analizerFontName", analizerFontName);
    settings.setValue("cppExtentions", cppExtentions);

    QApplication::setOrganizationName(organizationName);
    QApplication::setApplicationVersion(applicationVersion);
    QApplication::setApplicationName(applicationName);
}
