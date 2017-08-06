#ifndef INVASIONLIST_H
#define INVASIONLIST_H

#include <QObject>
#include <QSqlQueryModel>
#include <QStringList>

class invasionlist : public QSqlQueryModel
{
    Q_OBJECT
public:
    explicit invasionlist(QObject *parent = 0);


     /*!
     * \brief The Roles enum Перечисляем все роли, которые будут использоваться в TableView
     */
    enum Roles {
        IdRole = Qt::UserRole + 1,
        InvasionNameRole,
        InvasionNameIntRole,
        InvasionTypeRole,
        InvasionTypeNameRole,
        InvasionDateTimeRole,
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
    void get_all_by_date(QString date_begin, QString date_end, QString time);    
};

#endif // INVASIONLIST_H
