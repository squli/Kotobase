#include "invasionlist.h"
#include "database.h"
#include <QSqlTableModel>
#include <QAbstractItemModel>

invasionlist::invasionlist(QObject *parent) :
    QSqlQueryModel(parent)
{

}

// Метод для получения данных из модели
QVariant invasionlist::data(const QModelIndex & index, int role) const {
    // Определяем номер колонки, адрес так сказать, по номеру роли
    QVariant value = QSqlQueryModel::data(index, role);
    if(role < Qt::UserRole)
    {
        value = QSqlQueryModel::data(index, role);
    }
    else
    {
        // Создаём индекс с помощью новоиспечённого ID колонки
        int columnIdx = role - Qt::UserRole - 1;
        QModelIndex modelIndex = this->index(index.row(), columnIdx);
        /* И с помощью уже метода data() базового класса
         * вытаскиваем данные для таблицы из модели
         * */
        value = QSqlQueryModel::data(modelIndex, Qt::DisplayRole);
    }
    return value;
}


// Метод для получения имен ролей через хешированную таблицу.
QHash<int, QByteArray> invasionlist::roleNames() const {
    /* То есть сохраняем в хеш-таблицу названия ролей
     * по их номеру
     *  - это список номеров столбцов в результатах запроса updateModel, т.е. первый выбранный столбец это роль 1, второй - роль 2 и т.д.*/
    QHash<int, QByteArray> roles;
        roles[IdRole] = "id";
        //roles[PatientIdRole] = INV_PATIENT_ID;
        roles[InvasionNameRole] = TABLE_KORONAROGR_NAME;
        roles[InvasionNameIntRole] = INV_INVASION_NAME;
        roles[InvasionTypeRole] = INV_INVASION_TYPE;
        roles[InvasionTypeNameRole] = TABLE_INVASIONTYPES_FNAME;
        roles[InvasionDateTimeRole] = INV_INVASION_DATETIME;
    return roles;
}


// Метод обновления таблицы в модели представления данных
void invasionlist::updateModel(int patient_id)
{
    qDebug() << "updateModel() querry: ";
    QString querry = QString("SELECT "
                        TABLE_INVASIONS_NAME "." TABLE_ID ","
                        TABLENAME_KORONAROGR "." TABLE_KORONAROGR_NAME " , "
                        INV_INVASION_NAME " , "
                        INV_INVASION_TYPE " , "
                        TABLE_INVASIONTYPES_NAME "." TABLE_INVASIONTYPES_FNAME " , \
                        strftime('%d.%m.%Y %H:%M', " INV_INVASION_DATETIME ") as " INV_INVASION_DATETIME
                        " FROM " "(" TABLE_INVASIONS_NAME " JOIN " TABLENAME_KORONAROGR " ON " TABLE_INVASIONS_NAME "." INV_INVASION_NAME " = " TABLENAME_KORONAROGR "." TABLE_ID " ) \
                             JOIN " TABLE_INVASIONTYPES_NAME " ON " TABLE_INVASIONS_NAME "." INV_INVASION_TYPE " = " TABLE_INVASIONTYPES_NAME "." TABLE_ID
                        " WHERE " TABLE_INVASIONS_NAME "." INV_PATIENT_ID " = " + QString::number(patient_id) + " ORDER BY " INV_INVASION_DATETIME " DESC" );

    qDebug() << querry;
    this->setQuery(querry);
}

/*!
 * \brief Получение id из строки в модели представления данных
 */
int invasionlist::getId(int row)
{
    qDebug() << "invasionlist::getId(" << row << ") = " << this->data(this->index(row, 0), IdRole).toInt();    
    return this->data(this->index(row, 0), IdRole).toInt();
}

/*!
 * \brief invasionlist::get_all_by_date
 * \param date
 */
void invasionlist::get_all_by_date(QString date_begin, QString date_end, QString time)
{
    qDebug() << "updateModel() querry: ";
    QString querry = QString("SELECT "
                        TABLE_INVASIONS_NAME "." TABLE_ID ","
                        TABLENAME_KORONAROGR "." TABLE_KORONAROGR_NAME ", "
                        INV_INVASION_NAME ", "
                        INV_INVASION_TYPE " , "
                        TABLE_INVASIONTYPES_NAME "." TABLE_INVASIONTYPES_FNAME " , \
                        strftime('%d.%m.%Y', " INV_INVASION_DATETIME ") as " INV_INVASION_DATETIME
                        " FROM " "(" TABLE_INVASIONS_NAME " JOIN " TABLENAME_KORONAROGR " ON " TABLE_INVASIONS_NAME "." INV_INVASION_NAME " = " TABLENAME_KORONAROGR "." TABLE_ID " ) \
                             JOIN " TABLE_INVASIONTYPES_NAME " ON " TABLE_INVASIONS_NAME "." INV_INVASION_TYPE " = " TABLE_INVASIONTYPES_NAME "." TABLE_ID
                        " WHERE strftime('%d.%m.%Y', " INV_INVASION_DATETIME ") BETWEEN '" + date_begin + " " + time + "' AND '" + date_end + " " + time + "'");
    qDebug() << querry;
    this->setQuery(querry);
}
