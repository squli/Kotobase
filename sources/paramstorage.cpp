/*!


*/

#include "paramstorage.h"
#include <QFile>
#include <QDebug>

#include <QGuiApplication>
#include "db_consts.h"
#include <QDate>

ParamStorage *paramStorage = 0;




ParamStorage::ParamStorage(QObject *parent) : QObject(parent)
{

}

ParamStorage *ParamStorage::getInstance()
{
    if (!paramStorage) {
        paramStorage = new ParamStorage();

        //QDate dateToday = QDate::currentDate();

        ///заполнить значениями по умолчанию тут
        paramStorage->addParamInList(db_path_name_enum, PARAM_DB_PATH_NAME, QGuiApplication::applicationDirPath() + "/");
        paramStorage->addParamInList(db_file_name_enum, PARAM_DB_FILE_NAME, DATABASE_NAME);
        paramStorage->addParamInList(db_host_name_enum, PARAM_DB_HOST_NAME, DATABASE_HOSTNAME);
        paramStorage->addParamInList(rep_path_name_enum, PARAM_REP_PATH_NAME, QGuiApplication::applicationDirPath() + "/");
        paramStorage->addParamInList(rep_time_start_enum, PARAM_REP_TIME_START, "07:00");
        paramStorage->addParamInList(rep_time_stop_enum, PARAM_REP_TIME_STOP, "07:00");
        //paramStorage->addParamInList(rep_date_enum, PARAM_REP_DATE, dateToday.toString("dd_MM_yyyy"));
    }
    return paramStorage;
}

/*!
 * @brief ParamStorage::addParamInList
 * @param enumNum
 * @param paramName
 * @param value
 */
void ParamStorage::addParamInList(EParamsDefs enumNum, QString paramName, QString value)
{
    param_def_t *p_new_param = new param_def_t;

    p_new_param->enumNum = enumNum;
    p_new_param->paramName = paramName;
    p_new_param->paramValue = value;
    paramsList.insert(enumNum, p_new_param);
}

/*!
 * @brief ParamStorage::loadParams
 * @param fileName
 * @return
 */
bool ParamStorage::loadParams(QString fileName)
{
    QFile file(fileName);
    bool res = file.open(QIODevice::ReadOnly);
    if (!res) {
        qDebug() << "Can't open params file for reading conf values";
        return false;
    }

    QByteArray ba = file.readAll();
    file.close();

    QJsonParseError error;
    QJsonDocument confDoc = QJsonDocument::fromJson(ba, &error);

    if (error.error != 0)
    {
        qDebug() << "error while parsing json config data: " << error.errorString();
        return false;
    }

    QJsonValue confDefs = confDoc.object().value("confDefs");

    QJsonArray jArr(confDefs.toArray());

    for (QJsonArray::iterator i = jArr.begin(); i != jArr.end(); ++i)
    {
        QJsonObject obj = (*i).toObject();
        QVariantMap map = obj.toVariantMap();

        if (!map.contains("paramName"))
            continue; //если мы не можем определить,что это за параметр - переходим к следующему
        int parmNum = 0;
        if (parmNum == -1) continue; //не смогли найти параметр с таким именем
        EParamsDefs parm = (EParamsDefs)parmNum;
        if (!paramsList.contains(parm)) continue;
    }//for
    return true;
}

void ParamStorage::updateFromGui(QVariantMap params)
{
    qDebug() << "ParamStorage::updateFromGui(QVariantMap params)";
    this->paramsList[(EParamsDefs)0]->paramValue = params["db_path_name"].toString();
    this->paramsList[(EParamsDefs)1]->paramValue = params["db_file_name"].toString();
    this->paramsList[(EParamsDefs)2]->paramValue = params["db_host_name"].toString();
    this->paramsList[(EParamsDefs)3]->paramValue = params["rep_path_name"].toString();
    this->paramsList[(EParamsDefs)4]->paramValue = params["rep_time_start"].toString();
    this->paramsList[(EParamsDefs)5]->paramValue = params["rep_time_stop"].toString();

    saveParams(PARAM_FILE_NAME);
}


bool ParamStorage::saveParams(QString fileName) {

    QJsonObject obj;
    QJsonArray confDefs;

    QFile file(fileName);
    bool res = file.open(QIODevice::WriteOnly | QIODevice::Truncate);
    if (!res) {
        qDebug() << "Can't open file for writing conf values";
        return false;
    }

    for (int i = 0; i < paramsList.keys().count(); i++)
    {
        QJsonObject obj = paramsList.value(paramsList.keys().at(i))->toJsonObject();
        confDefs.append(obj);
    }//for i

    obj.insert("confDefs",confDefs);

    QJsonDocument confDoc(obj);
    QByteArray ba = confDoc.toJson();

    res = file.write(ba);

    if (!res) {
        qDebug() << "Can't save conf values";
    }

    file.close();

    ///make reconncet to db

    return res;
}

QString ParamStorage::getValueByEnum(int en)
{
    EParamsDefs enum_nuber = (EParamsDefs)en;
    param_def_t *v = paramsList.value(enum_nuber);
    return v->paramValue;
}
