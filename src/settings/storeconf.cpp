#include "storeconf.h"

#include <regex>

#include <QDebug>
#include <QString>
#include <QSettings>
#include <QJsonArray>
#include <QFileDialog>
#include <QApplication>

StoreConf::StoreConf(QWidget * parent)
    : QWidget (parent)
{
    conFile = getPathToConFile();
    if (!conFile.size()) // conf.json file not exist. keep all fields with default values,
    {                   // create conf.json with these values
        writeJson();
        saveData();
    } else {
        readJson();
        parseJson();
        saveData();
    }
}

QString StoreConf::getPathToConFile()
{
    QString currentPath = QDir::currentPath();
    currentPath += "/conf.json";
    QFile file(currentPath);
    if(file.exists())
        return "conf.json";
    else
        return "";
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

    cppExtentionsList.clear();

    std::string cppExtentionsS = cppExtentions.toStdString();
    std::vector<std::string> tokens;
    std::regex re("\\;+");
    std::sregex_token_iterator begin(cppExtentionsS.begin(), cppExtentionsS.end(), re, -1);
    std::sregex_token_iterator end;
    std::copy(begin, end, std::back_inserter(tokens));
    for (unsigned i = 0; i < tokens.size() ; i++)
        cppExtentionsList << tokens[i].c_str();
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
    settings.setValue("cppExtentionsList", QVariant::fromValue(cppExtentionsList));

    QApplication::setOrganizationName(organizationName);
    QApplication::setApplicationVersion(applicationVersion);
    QApplication::setApplicationName(applicationName);
}
