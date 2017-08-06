/*!
    \brief Класс для работы с хранилищем параметров
*/

#ifndef PARAMSTORAGE_H
#define PARAMSTORAGE_H

#include <QObject>
#include <QVariantMap>
#include <QJsonArray>
#include <QJsonParseError>
#include <QJsonObject>

#define PARAM_FILE_NAME "params.conf"

#define PARAM_DB_PATH_NAME      "db_path_name"
#define PARAM_DB_FILE_NAME      "db_file_name"
#define PARAM_DB_HOST_NAME      "db_host_name"
#define PARAM_REP_PATH_NAME     "rep_path_name"
#define PARAM_REP_TIME_START    "rep_time_start"
#define PARAM_REP_TIME_STOP     "rep_time_stop"
#define PARAM_REP_DATE          "rep_date"

typedef enum {
    db_path_name_enum = 0,
    db_file_name_enum,
    db_host_name_enum,
    rep_path_name_enum,
    rep_time_start_enum,
    rep_time_stop_enum,
    rep_date_enum,
} EParamsDefs;

typedef struct {
    EParamsDefs enumNum;
    QString paramName;
    QString paramValue;

    QVariantMap toVariantMap() {
        QVariantMap map;
        map.insert("enumNum", enumNum);
        map.insert("paramName", paramName);
        map.insert("paramValue", paramValue);
        return map;
    }

    //возвращает JSON object из данных структуры
    QJsonObject toJsonObject() {
        QVariantMap map=toVariantMap();
        QJsonObject obj = QJsonObject::fromVariantMap(map);
        return obj;
    }
} param_def_t;

class ParamStorage : public QObject
{
    Q_OBJECT
public:
    explicit ParamStorage(QObject *parent = 0);
    static ParamStorage *getInstance();
    void addParamInList(EParamsDefs enumNum, QString paramName, QString value);
    bool loadParams(QString filename);
    bool saveParams(QString filename);

private:
    QMap<EParamsDefs, param_def_t*> paramsList;

signals:

public slots:
    void updateFromGui(QVariantMap params);
    QString getValueByEnum(int en);
};

#endif // PARAMSTORAGE_H
