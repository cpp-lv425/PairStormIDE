#include "storeconf.h"

#include <regex>

#include <QDebug>
#include <QString>
#include <QSettings>
#include <QJsonArray>
#include <QFileDialog>
#include <QApplication>

StoreConf::StoreConf(QString userName)
{
    mConFile = userName;
    mConFile += ".json";
    mFields["applicationName"]      = "Pair Storm";       //  default values used when json file
    mFields["applicationVersion"]   = "0.1";           //      not exist or corrupted
    mFields["organizationName"]     = "Lv-425.C++";
    mFields["style"]                = "WHITE";
    mFields["editorFontName"]       = "Consolas";
    mFields["editorFontSize"]       = "12";
    mFields["guiFontName"]          = "Consolas";
    mFields["guiFontSize"]          = "12";
    mFields["defaultUdpPortNumber"] = "36108";
    mFields["defaultTcpPortNumber"] = "32807";
    mFields["userName"]             = userName;
    mFields["cppStandart"]          = "C++11";
    mFields["cppExtentions"]        = ".c;.cpp;.h;.hpp;.json;.txt";
    mFields["cppCompilers"]         = "";
    mFields["cppLibraryHeaders"]    = "";
    mFields["cppLibraryBinarys"]    = "";
    separateList(mFields["cppExtentions"], mCppExtentionsList);
}

void StoreConf::restoreConFile()
{
    checkConfDir();
    getPathToConFile();
    QFile file(mPathToConFile);
    if (!file.exists()) // userName.json file not exist. keep all fields with default values,
    {                   // create userName.json with these values
        writeJson();
    }
    else
    {
        readJson();
        if (mReadStatus)
        {
            parseJson();
        }
        else
        {                        //  file corrupted
            writeJson();        //      rewrite with default values
        }
    }
    saveData();
}

void StoreConf::saveConFile()
{
    getPathToConFile();
    QFile file(mPathToConFile);
    if (file.exists())
    {
        writeJson(sessionMode::Finish);
    }
}

void StoreConf::getPathToConFile()
{
    mPathToConFile = QDir::currentPath();
    mPathToConFile += QDir::separator();
    mPathToConFile += "conf";
    mPathToConFile += QDir::separator();
    mPathToConFile += mConFile;
}

void StoreConf::checkConfDir(QString str)
{
    if (str == "exe")
    {
        QString pathToConDir = QDir::currentPath();
        pathToConDir += "/conf";
        if (!QDir(pathToConDir).exists())
            QDir().mkdir(pathToConDir);
    }
}

void StoreConf::writeJson(sessionMode mode)
{
    QJsonObject root_obj;

    QSettings s;
    if(mode == sessionMode::Finish)
    {
        if (s.contains("cppExtentionsList"))
        {
            QString strExtentions;
            QStringList extentionList = s.value("cppExtentionsList").toStringList();
            for (int i = 0; i < extentionList.size(); ++i)
            {
                if(i != 0)
                    strExtentions += ";";
                strExtentions += extentionList.at(i);
            }
            s.setValue("cppExtentions", strExtentions);
        }

        if (s.contains("cppCompilersList"))
        {
            QString strCompilers;
            QStringList compilersList = s.value("cppCompilersList").toStringList();
            for (int i = 0; i < compilersList.size(); ++i)
            {
                if(i != 0)
                    strCompilers += ";";
                strCompilers += compilersList.at(i);
            }
            s.setValue("cppCompilers", strCompilers);
        }

        if (s.contains("cppLibraryHeadersList"))
        {
            QString strHeaders;
            QStringList headersList = s.value("cppLibraryHeadersList").toStringList();
            for (int i = 0; i < headersList.size(); ++i)
            {
                if(i != 0)
                    strHeaders += ";";
                strHeaders += headersList.at(i);
            }
            s.setValue("cppLibraryHeaders", strHeaders);
        }

        if (s.contains("cppLibraryBinarysList"))
        {
            QString strBinarys;
            QStringList binarysList = s.value("cppLibraryBinarysList").toStringList();
            for (int i = 0; i < binarysList.size(); ++i)
            {
                if(i != 0)
                    strBinarys += ";";
                strBinarys += binarysList.at(i);
            }
            s.setValue("cppLibraryBinarys", strBinarys);
        }
    }
    QMap<QString, QString>::const_iterator it = mFields.constBegin();
    while (it != mFields.constEnd())
    {
        switch (mode)
        {
        case sessionMode::Start:
        {                               // copy value from QMap to json
            root_obj.insert(it.key(), it.value());
            break;
        }
        case sessionMode::Finish:
        {                               // copy value from QSettings to json
            if (s.contains(it.key()))
                root_obj.insert(it.key(), s.value(it.key()).toString());
            break;
        }
        }
        ++it;
    }

    QJsonDocument json_doc(root_obj);
    QString json_string = json_doc.toJson();

    QFile save_file(mPathToConFile);
    if(!save_file.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        return;
    }

    save_file.write(json_string.toLocal8Bit());
}

void StoreConf::readJson()
{
    QFile loadFile(mPathToConFile);                       // closing in it destructor
    if (loadFile.open(QIODevice::ReadOnly))
    {
        mReadStatus = true;
    }
    else
    {
        mReadStatus = false;
        return;
    }

    QByteArray saveData = loadFile.readAll();
    mJsonDoc = QJsonDocument::fromJson(saveData);
    if(mJsonDoc.isNull() || !mJsonDoc.isObject() || mJsonDoc.isEmpty())
        mReadStatus = false;
    else
        mReadStatus = true;
}

void StoreConf::parseJson()
{
    QJsonObject json = mJsonDoc.object();
    QMap<QString, QString>::const_iterator it = mFields.constBegin();
    while (it != mFields.constEnd())
    {
        if (json.contains(it.key()) && json[it.key()].isString())   // overwrite default value
            mFields[it.key()] = json[it.key()].toString();           // from json to QMap
        ++it;
    }

    mCppExtentionsList.clear();                                      // overwrite default value
    QString cppExtentions = mFields["cppExtentions"];
    separateList(cppExtentions, mCppExtentionsList);

    mCppCompilersList.clear();                                      // overwrite default value
    QString mCppCompilers = mFields["cppCompilers"];
    separateList( mCppCompilers, mCppCompilersList);

    mCppLibraryHeadersList.clear();                                      // overwrite default value
    QString mCppLibraryHeaders = mFields["cppLibraryHeaders"];
    separateList(mCppLibraryHeaders, mCppLibraryHeadersList);

    mCppLibraryBinarysList.clear();                                      // overwrite default value
    QString mCppLibraryBinarys = mFields["cppLibraryBinarys"];
    separateList(mCppLibraryBinarys, mCppLibraryBinarysList);

}

void StoreConf::separateList(QString &str, QStringList &lst)
{
    std::string cppExtentionsS = str.toStdString();
    std::vector<std::string> tokens;
    std::regex re("\\;+");
    std::sregex_token_iterator begin(cppExtentionsS.begin(), cppExtentionsS.end(), re, -1);
    std::sregex_token_iterator end;
    std::copy(begin, end, std::back_inserter(tokens));
    for (unsigned i = 0; i < tokens.size() ; i++)
        lst << tokens[i].c_str();
}

void StoreConf::saveData()
{
    QApplication::setOrganizationName(mFields["organizationName"]);
    QApplication::setApplicationVersion(mFields["applicationVersion"]);
    QApplication::setApplicationName(mFields["applicationName"]);

    QSettings settings;
    QMap<QString, QString>::const_iterator it = mFields.constBegin();
    while (it != mFields.constEnd())
    {
        settings.setValue(it.key(), mFields[it.key()]);        // copy values from QMap to QSettings
        ++it;
    }
    settings.setValue("cppExtentionsList", QVariant::fromValue(mCppExtentionsList));
    settings.setValue("cppCompilersList", QVariant::fromValue(mCppCompilersList));
    settings.setValue("cppLibraryHeadersList", QVariant::fromValue(mCppLibraryHeadersList));
    settings.setValue("cppLibraryBinarysList", QVariant::fromValue(mCppLibraryBinarysList));
}
