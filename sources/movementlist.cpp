#include "movementlist.h"
#include "database.h"
#include <QSqlTableModel>
#include <QAbstractItemModel>

MovementList::MovementList(QObject *parent) :
    QSqlQueryModel(parent)
{

}


// Метод для получения данных из модели
QVariant MovementList::data(const QModelIndex & index, int role) const {
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
QHash<int, QByteArray> MovementList::roleNames() const {
    /* То есть сохраняем в хеш-таблицу названия ролей
     * по их номеру
     * */
    QHash<int, QByteArray> roles;
        roles[IdRole] = "id";
        //roles[PatientIdRole] = INV_PATIENT_ID;
        roles[NewStorageNameRole] = MOV_NEW_STORAGE;
        roles[MovementDateTimeRole] = MOV_DATETIME;
    return roles;
}

// Метод обновления таблицы в модели представления данных
void MovementList::updateModel(int patient_id)
{
    qDebug() << "updateModel() querry: ";
    QString querry = QString("SELECT "
                        TABLE_MOVEMENTS_NAME "." TABLE_ID ", "
                        TABLE_STORAGES_NAME "." STO_STORAGES_NAME ", \
                        strftime('%d.%m.%Y %H:%M', "MOV_DATETIME") as " MOV_DATETIME
                        " FROM " "(" TABLE_MOVEMENTS_NAME " JOIN " TABLE_STORAGES_NAME " ON " TABLE_MOVEMENTS_NAME "." MOV_NEW_STORAGE " = " TABLE_STORAGES_NAME "." TABLE_ID " ) "
                        " WHERE " TABLE_MOVEMENTS_NAME "." INV_PATIENT_ID " = " + QString::number(patient_id) + " ORDER BY " MOV_DATETIME " DESC");

    qDebug() << querry;
    this->setQuery(querry);
}

/*!
 * \brief Получение id из строки в модели представления данных
 */
int MovementList::getId(int row)
{
    qDebug() << "MovementList::getId(" << row << ") = " << this->data(this->index(row, 0), IdRole).toInt();
    return this->data(this->index(row, 0), IdRole).toInt();
}

/*!
 * \brief MovementList::getLastStorage
 * \param patient_id
 * \return
 */
void MovementList::getLastStorage(int patient_id)
{
    qDebug() << "updateModel() querry: ";
    QString querry = QString("SELECT "
                        TABLE_MOVEMENTS_NAME "." TABLE_ID ", "
                        TABLE_STORAGES_NAME "." STO_STORAGES_NAME ", \
                        strftime('%d.%m.%Y %H:%M', "MOV_DATETIME") as " MOV_DATETIME
                        " FROM " "(" TABLE_MOVEMENTS_NAME " JOIN " TABLE_STORAGES_NAME " ON " TABLE_MOVEMENTS_NAME "." MOV_NEW_STORAGE " = " TABLE_STORAGES_NAME "." TABLE_ID " ) "
                        " WHERE " TABLE_MOVEMENTS_NAME "." INV_PATIENT_ID " = " + QString::number(patient_id) + " ORDER BY " MOV_DATETIME " DESC LIMIT 1");

    qDebug() << querry;
    this->setQuery(querry);
}


