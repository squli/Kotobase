#include "listmodel.h"
#include "database.h"
#include <QSqlTableModel>
#include <QAbstractItemModel>

ListModel::ListModel(QObject *parent):
    QSqlQueryModel(parent)
{
    this->updateModel();
}

// Метод для получения данных из модели
QVariant ListModel::data(const QModelIndex & index, int role) const
{
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
QHash<int, QByteArray> ListModel::roleNames() const {
    /* То есть сохраняем в хеш-таблицу названия ролей
     * по их номеру
     * */
    QHash<int, QByteArray> roles;
        roles[IdRole] = "id";
        roles[FNameRole] = TABLE_FNAME;
        roles[SNameRole] = TABLE_SNAME;
        roles[PatRole] = TABLE_PNAME;
        roles[AgeRole] = TABLE_AGE;
        roles[HistoryNumberRole] = TABLE_HISTORYNUMBER;
        roles[HospitalDateRole] = TABLE_HOSPITALDATE;
        roles[diagnozOKSRole] = TABLE_DIAGNOZ_OKS;
        roles[outcomeRole] = TABLE_OUTCOME;
        roles[outcomeNameRole] = TABLE_OUTCOME_JOIN ;
        roles[outcomeDateRole] = TABLE_OUTCOMEDATE;
    return roles;
}

// Метод обновления таблицы в модели представления данных
void ListModel::updateModel()
{
    // Обновление производится SQL-запросом к базе данных
    qDebug() << "updateModel() querry: ";
    QString querry = QString("SELECT "
                        TABLE_PATIENTS "." TABLE_ID ", "
                        TABLE_FNAME ", "
                        TABLE_SNAME ", "
                        TABLE_PNAME ", "
                        TABLE_AGE ", "
                        TABLE_HISTORYNUMBER ", \
                        strftime('%d.%m.%Y', " TABLE_HOSPITALDATE ") as " TABLE_HOSPITALDATE ", "
                        TABLE_DIAGNOZ_OKS ", "
                        TABLE_OUTCOME " , "
                        TABLENAME_OUTCOME "." TABLE_OUTCOME_JOIN ", \
                        strftime('%d.%m.%Y %H:%M', "TABLE_OUTCOMEDATE") as " TABLE_OUTCOMEDATE
                        " FROM " "(" TABLE_PATIENTS " JOIN " TABLENAME_OUTCOME " ON " TABLE_PATIENTS "." TABLE_OUTCOME " = " TABLE_OUTCOME "." TABLE_ID " ) ");
    qDebug() << querry;
    this->setQuery(querry);
}

/*!
 * \brief ListModel::actual_patients_updateModel Метод обновления таблицы для отображения находящихся в настоящий момент пациентов
 */
void ListModel::actual_patients_updateModel()
{
    // Обновление производится SQL-запросом к базе данных
    qDebug() << "actual_patients_updateModel() querry: ";
    QString querry = QString("SELECT "
                        TABLE_PATIENTS "." TABLE_ID ", "
                        TABLE_FNAME ", "
                        TABLE_SNAME ", "
                        TABLE_PNAME ", "
                        TABLE_AGE ", "
                        TABLE_HISTORYNUMBER ", \
                        strftime('%d.%m.%Y', " TABLE_HOSPITALDATE ") as " TABLE_HOSPITALDATE ", "
                        TABLE_DIAGNOZ_OKS ", "
                        TABLE_OUTCOME " , "
                        TABLENAME_OUTCOME "." TABLE_OUTCOME_JOIN ", \
                        strftime('%d.%m.%Y %H:%M', " TABLE_OUTCOMEDATE ") as " TABLE_OUTCOMEDATE
                        " FROM " "(" TABLE_PATIENTS " JOIN " TABLENAME_OUTCOME " ON " TABLE_PATIENTS "." TABLE_OUTCOME " = " TABLE_OUTCOME "." TABLE_ID " ) " " WHERE " TABLE_OUTCOME ACTUAL_PATIENT_LIST_CRIT);
    qDebug() << querry;
    this->setQuery(querry);
}

//Заполнение модели пациентами, имеющими дату исхода в пределах 24 часов от даты госпитализации по параметру даты госпитализации
// исход - смерть
void ListModel::first_day_letalnost_updateModel(QString date)
{
    // Обновление производится SQL-запросом к базе данных
    qDebug() << "first_day_letalnost_updateModel() querry: ";
    QString querry = QString("SELECT "
                        TABLE_PATIENTS "." TABLE_ID ", "
                        TABLE_FNAME ", "
                        TABLE_SNAME ", "
                        TABLE_PNAME ", "
                        TABLE_AGE ", "
                        TABLE_HISTORYNUMBER ", \
                        strftime('%d.%m.%Y %H:%M', " TABLE_HOSPITALDATE ") as " TABLE_HOSPITALDATE ", "
                        TABLE_DIAGNOZ_OKS ", "
                        TABLE_OUTCOME " , "
                        TABLENAME_OUTCOME "." TABLE_OUTCOME_JOIN ", \
                        strftime('%d.%m.%Y %H:%M', " TABLE_OUTCOMEDATE ") as " TABLE_OUTCOMEDATE
                        " FROM " "(" TABLE_PATIENTS " JOIN " TABLENAME_OUTCOME " ON " TABLE_PATIENTS "." TABLE_OUTCOME " = " TABLE_OUTCOME "." TABLE_ID " ) \
                        WHERE strftime('%d.%m.%Y %H:%M', " TABLE_OUTCOMEDATE ") BETWEEN strftime('%d.%m.%Y %H:%M', " TABLE_HOSPITALDATE ") AND strftime('%d.%m.%Y %H:%M', " TABLE_HOSPITALDATE ", '+1 day') \
                                AND strftime('%d.%m.%Y', " TABLE_HOSPITALDATE ") = '" + date + "'" +
                                " AND " TABLE_OUTCOME " = 1 ");
    qDebug() << querry;
    this->setQuery(querry);
}

//Заполнение модели пациентами, имеющими дату исхода в пределах 24 часов от даты госпитализации по параметру даты госпитализации
// исход - перевод
void ListModel::first_day_perevod_updateModel(QString date)
{
    // Обновление производится SQL-запросом к базе данных
    qDebug() << "first_day_perevod_updateModel() querry: ";
    QString querry = QString("SELECT "
                        TABLE_PATIENTS "." TABLE_ID ", "
                        TABLE_FNAME ", "
                        TABLE_SNAME ", "
                        TABLE_PNAME ", "
                        TABLE_AGE ", "
                        TABLE_HISTORYNUMBER ", \
                        strftime('%d.%m.%Y %H:%M', " TABLE_HOSPITALDATE ") as " TABLE_HOSPITALDATE ", "
                        TABLE_DIAGNOZ_OKS ", "
                        TABLE_OUTCOME " , "
                        TABLENAME_OUTCOME "." TABLE_OUTCOME_JOIN ", \
                        strftime('%d.%m.%Y %H:%M', " TABLE_OUTCOMEDATE ") as " TABLE_OUTCOMEDATE
                        " FROM " "(" TABLE_PATIENTS " JOIN " TABLENAME_OUTCOME " ON " TABLE_PATIENTS "." TABLE_OUTCOME " = " TABLE_OUTCOME "." TABLE_ID " ) \
                        WHERE strftime('%d.%m.%Y %H:%M', " TABLE_OUTCOMEDATE ") BETWEEN strftime('%d.%m.%Y %H:%M', " TABLE_HOSPITALDATE ") AND strftime('%d.%m.%Y %H:%M', " TABLE_HOSPITALDATE ", '+1 day') \
                                AND strftime('%d.%m.%Y', " TABLE_HOSPITALDATE ") = '" + date + "'" +
                                " AND " TABLE_OUTCOME " = 2 ");
    qDebug() << querry;
    this->setQuery(querry);
}

/*!
 * \brief Получение id из строки в модели представления данных
 */
int ListModel::getId(int row)
{
    qDebug() << "ListModel::getId(" << row << ") = " << this->data(this->index(row, 0), IdRole).toInt();
    return this->data(this->index(row, 0), IdRole).toInt();
}

/*!
 * \brief Получение данных по номеру ряда в модели для заполнения полей ввода данных при просмотре таблицы
 */
QVariantMap ListModel::get(int row_number) const
{
  QVariantMap map;
  QModelIndex ix = QAbstractItemModel::createIndex(row_number, 0);

  qDebug() << "QModelIndex ix = " << ix;

  foreach(int k, roleNames().keys()) {
    map[roleNames().value(k)] = data(ix, k);
  }
  return map;
}



