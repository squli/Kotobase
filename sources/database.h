/*!
    \brief В данном классе объявлены методы для работы с базой данных.

    методы для подключения к базе данных, её восстановления;
    методы для добавления записи в базу данных;
    методы для удаления записей в таблице данных.
*/

#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSql>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>
#include <QFile>
#include <QDate>
#include <QDebug>

#include <db_consts.h>

#include "paramstorage.h"


#define ACTUAL_PATIENT_LIST_CRIT    " = 0"  //значение исхода 0, т.е. "не переведен"
/*!
 * \brief The DataBase class
 Класс для работы с базой данных: подключение к базе, вставка, удаление, модификация данных.
 */
class DataBase : public QObject
{
    Q_OBJECT
public:
    /// Конструктор по умолчанию
    explicit DataBase(QObject *parent = 0);
    ~DataBase();
    /// Флаг установки подключения к базе данных
    bool connection_to_db_done;

private:
    /// Сам объект базы данных, с которым будет производиться работа
    QSqlDatabase    db;

private:
    /// Открытие базы данных
    bool openDataBase();


signals:

public slots:
    /// Подключение к базе данных
    bool connectToDataBase();
    /// Закрытие базы данных
    void closeDataBase();

    bool get_connection_to_db_done(void);

    /// Добавление записей в таблицу пациентов
    bool inserIntoTablePatients(const QVariantMap &data);
    /// Добавление записей в таблицу манипуляций
    bool inserIntoTableInvasions(const QVariantMap &data);
    /// Добавление записей в таблицу перемещений
    bool inserIntoTableMovements(const QVariantMap &data);
    /// Редактирование записи
    bool updateRecord(const int id, const QVariantMap &data);
    /// Удаление записи из таблицы по её id
    bool removeRecord(QString table_name, const int id);
};

#endif // DATABASE_H
