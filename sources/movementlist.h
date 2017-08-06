#ifndef MOVEMENTLIST_H
#define MOVEMENTLIST_H

#include <QObject>
#include <QSqlQueryModel>
#include <QStringList>

class MovementList : public QSqlQueryModel
{
    Q_OBJECT
public:
    explicit MovementList(QObject *parent = 0);
    /*!
    * \brief The Roles enum Перечисляем все роли, которые будут использоваться в TableView
    */
   enum Roles {
       IdRole = Qt::UserRole + 1,      // id
       //PatientIdRole,
       NewStorageNameRole,
       MovementDateTimeRole,
   };

   /*!
    * \brief data Переопределяем метод, который будет возвращать данные
    * \param index
    * \param role
    * \return
    */
   QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

protected:
    /* хешированная таблица ролей для колонок.
     * Метод используется в дебрях базового класса QAbstractItemModel,
     * от которого наследован класс QSqlQueryModel
     * */
    QHash<int, QByteArray> roleNames() const;

signals:

public slots:
    /*!
     * \brief updateModel Заполняет модель данными о работе с определенным пациентом
     * \param patient_id
     */\
    void updateModel(int patient_id);
    int getId(int row);
    /*!
     * \brief MovementList::getLastStorage
     * \param patient_id
     * \return
     */
    void getLastStorage(int patient_id);
};

#endif // MOVEMENTLIST_H
