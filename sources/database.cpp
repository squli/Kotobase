#include "database.h"

///Конструктор по умолчанию
DataBase::DataBase(QObject *parent) : QObject(parent)
{
    connection_to_db_done = false;
}

///Деструктор по умолчанию
DataBase::~DataBase()
{

}

bool DataBase::get_connection_to_db_done(void)
{
    return connection_to_db_done;
}

/*!
 * \brief Методы для подключения к базе данных
 * \return true, если подключиться удалось
 * */
bool DataBase::connectToDataBase()
{
    bool res = false;
    /* Перед подключением к базе данных производим проверку на её существование.
     * В зависимости от результата производим открытие базы данных или её восстановление
     * */
    QString str = ParamStorage::getInstance()->getValueByEnum(db_path_name_enum) + ParamStorage::getInstance()->getValueByEnum(db_file_name_enum);
    if(QFile(str).exists())
    {
        res = this->openDataBase();
    }
    else
    {
        qCritical() << "Не обнаружен файл базы данных по адресу: " << str;
    }

    if (res == true)
        connection_to_db_done = true;

    return res;
}


/*!
 * \brief Метод для открытия базы данных
 * */
bool DataBase::openDataBase()
{
    /* База данных открывается по заданному пути
     * и имени базы данных, если она существует
     * */
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setHostName(DATABASE_HOSTNAME);
    db.setDatabaseName(ParamStorage::getInstance()->getValueByEnum(db_path_name_enum) + ParamStorage::getInstance()->getValueByEnum(db_file_name_enum));
    if(db.open()){
        return true;
    } else {
        return false;
    }
}

/*!
 * \brief Методы закрытия базы данных
 * */
void DataBase::closeDataBase(void)
{
    db.close();
}

/*!
 * \brief Метод для вставки записи в базу данных
 * \param &data - ссылка на QVariantMap с новыми данными
 * */
bool DataBase::inserIntoTablePatients(const QVariantMap &data)
{
    /* Запрос SQL формируется из QVariantList,
     * в который передаются данные для вставки в таблицу.
     * */
    QSqlQuery query;
    /* В начале SQL запрос формируется с ключами,
     * которые потом связываются методом bindValue
     * для подстановки данных из QVariantList
     * */
    query.prepare("INSERT INTO " TABLE_PATIENTS " ( "
                                                  TABLE_FNAME             ", "
                                                  TABLE_SNAME             ", "
                                                  TABLE_PNAME             ", "
                                                  TABLE_AGE               ", "
                                                  TABLE_HISTORYNUMBER     ", "
                                                  TABLE_HOSPITALDATE      ", "                                                                                                   
                                                  TABLE_DIAGNOZ_OKS       ", "
                                                  TABLE_OUTCOME           ", "
                                                  TABLE_OUTCOMEDATE
                                                " ) "
                  "VALUES (:surname,:name,:pat,:age,:history_num,:hospitaliz_date,:diagnoz_oks,:outcome,:outcome_date)");

    query.bindValue(":surname",         data["surname"].toString());
    query.bindValue(":name",            data["name"].toString());
    query.bindValue(":pat",             data["pat"].toString());
    query.bindValue(":age",             data["age"]);
    query.bindValue(":history_num",     data["history_num"].toString());
    query.bindValue(":hospitaliz_date", data["hospitaliz_date"].toString());
    query.bindValue(":diagnoz_oks",     data["diagnoz_oks"].toString());
    query.bindValue(":outcome",         data["outcome"]);
    query.bindValue(":outcome_date",    data["outcome_date"]);

    // После чего выполняется запросом методом exec()
    if(!query.exec()){
        qDebug() << "error insert into " << TABLE_PATIENTS;
        qDebug() << query.lastError().text();
        return false;
    } else {
        return true;
    }
    return false;
}

/*!
 * \brief inserIntoTableInvasions Добавление данных в таблицу процедур
 * \param data
 * \return
 */
bool DataBase::inserIntoTableInvasions(const QVariantMap &data)
{
    QSqlQuery query;

    query.prepare("INSERT INTO " TABLE_INVASIONS_NAME " ( "
                                                  INV_PATIENT_ID        ", "
                                                  INV_INVASION_NAME     ", "
                                                  INV_INVASION_TYPE     ", "
                                                  INV_INVASION_DATETIME " ) "
                        "VALUES (:id_pat,:invasion_name,:invasion_type,:invasion_datetime)");

    query.bindValue(":id_pat",               data["id_pat"]);
    query.bindValue(":invasion_name",        data["invasion_name"]);
    query.bindValue(":invasion_type",        data["invasion_type"].toString());
    query.bindValue(":invasion_datetime",    data["invasion_datetime"].toString());

    // После чего выполняется запросом методом exec()
    if(!query.exec()){
        qDebug() << "error insert into " << TABLE_INVASIONS_NAME;
        qDebug() << query.lastError().text();
        return false;
    } else {
        return true;
    }
    return false;
}
/*!
 * \brief inserIntoTableMovements Добавление данных в таблицу перемещений
 * \param data
 * \return
 */
bool DataBase::inserIntoTableMovements(const QVariantMap &data)
{
    QSqlQuery query;

    query.prepare("INSERT INTO " TABLE_MOVEMENTS_NAME " ( "
                                                  MOV_ID_PAT         ", "
                                                  MOV_NEW_STORAGE     ", "
                                                  MOV_DATETIME      " ) "
                        "VALUES (:id_pat,:new_storage,:movement_datetime)");

    query.bindValue(":id_pat",               data["id_pat"]);
    query.bindValue(":new_storage",        data["new_storage"]);
        query.bindValue(":movement_datetime",    data["movement_datetime"].toString());

    // После чего выполняется запросом методом exec()
    if(!query.exec()){
        qDebug() << "error insert into " << TABLE_MOVEMENTS_NAME;
        qDebug() << query.lastError().text();
        return false;
    } else {
        return true;
    }
    return false;
}

/*!
 * \brief Метод для удаления записи из таблицы
 * \param id - уникальный идентификатор записи в таблице
 * */
bool DataBase::removeRecord(QString table_name, const int id)
{
    // Удаление строки из базы данных будет производитсья с помощью SQL-запроса
    QSqlQuery query;

    // Удаление производим по id записи, который передается в качестве аргумента функции
    query.prepare("DELETE FROM " + table_name + " WHERE id = :ID ;");
    query.bindValue(":ID", id);

    //при удалении пациента удаляются и ссылки на него
    if (table_name == TABLE_PATIENTS)
    {
        QSqlQuery query_inv;
        query_inv.prepare("DELETE FROM " TABLE_INVASIONS_NAME " WHERE " INV_PATIENT_ID "= :ID ;");
        query_inv.bindValue(":ID", id);

        if(!query_inv.exec()){
            qDebug() << "error delete row " << TABLE_INVASIONS_NAME;
            qDebug() << query_inv.lastError().text();
            return false;
        }

        QSqlQuery query_mov;
        query_mov.prepare("DELETE FROM " TABLE_MOVEMENTS_NAME " WHERE " MOV_ID_PAT "= :ID ;");
        query_mov.bindValue(":ID", id);

        if(!query_mov.exec()){
            qDebug() << "error delete row " << TABLE_MOVEMENTS_NAME;
            qDebug() << query_mov.lastError().text();
            return false;
        }
    }

    // Выполняем удаление
    if(!query.exec()){
        qDebug() << "error delete row " << table_name;
        qDebug() << query.lastError().text();
        return false;
    } else {
        return true;
    }
    return false;
}


/*!
 * \brief Метод для редактирования записи
 * \param [in] id идентификатор запсии в базе (ключ таблицы с пациентами)
 * \param [in] data - новый набор данных для записи
 */
bool DataBase::updateRecord(const int id, const QVariantMap &data)
{
    bool res = false;

    QSqlQuery query;

    query.prepare("UPDATE " TABLE_PATIENTS " SET "
                                                  TABLE_FNAME          "= :surname ,"
                                                  TABLE_SNAME          "= :name ,"
                                                  TABLE_PNAME          "= :pat ,"
                                                  TABLE_AGE            "= :age ,"
                                                  TABLE_HISTORYNUMBER  "= :history_num ,"
                                                  TABLE_HOSPITALDATE   "= :hospitaliz_date ,"                                                  
                                                  TABLE_DIAGNOZ_OKS    "= :diagnoz_oks ,"                                                  
                                                  TABLE_OUTCOME        "= :outcome ,"
                                                  TABLE_OUTCOMEDATE    "= :outcome_date"
                                            " WHERE " TABLE_ID       " = :where_val"  );

    query.bindValue(":surname",         data["surname"].toString());
    query.bindValue(":name",            data["name"].toString());
    query.bindValue(":pat",             data["pat"].toString());
    query.bindValue(":age",             data["age"]);
    query.bindValue(":history_num",     data["history_num"].toString());
    query.bindValue(":hospitaliz_date", data["hospitaliz_date"].toString());
    query.bindValue(":diagnoz_oks",     data["diagnoz_oks"].toString());
    query.bindValue(":koronagrophia",   data["koronagrophia"]);
    query.bindValue(":outcome",         data["outcome"]);
    query.bindValue(":outcome_date",    data["outcome_date"]);
    query.bindValue(":where_val",       id);

    // После чего выполняется запросом методом exec()
    if(!query.exec()){
        qDebug() << "[ERR] update into " << TABLE_PATIENTS << ":id = " << id;
        qDebug() << query.lastError().text();
        res = false;
    } else {
        res = true;
    }

    return res;
}
