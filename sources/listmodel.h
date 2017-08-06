#ifndef LISTMODEL_H
#define LISTMODEL_H

#include <QObject>
#include <QSqlQueryModel>
#include <QStringList>

class ListModel : public QSqlQueryModel
{
    Q_OBJECT
public:
    // объявляем конструктор класса
    explicit  ListModel(QObject *parent = 0);

    /* Перечисляем все роли, которые будут использоваться в TableView
     * Как видите, они должны лежать в памяти выше параметра Qt::UserRole
     * Связано с тем, что информация ниже этого адреса не для кастомизаций
     * */
    enum Roles {
        IdRole = Qt::UserRole + 1,      // id
        FNameRole,                      // имя
        SNameRole,                      // фамилия
        PatRole,                        // отчество
        AgeRole,                        // возраст
        HistoryNumberRole,              // история болезни
        HospitalDateRole,
        diagnozOKSRole,
        outcomeRole,
        outcomeNameRole,
        outcomeDateRole,
    };

    // Переопределяем метод, который будет возвращать данные
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

protected:
    /* хешированная таблица ролей для колонок.
     * Метод используется в дебрях базового класса QAbstractItemModel,
     * от которого наследован класс QSqlQueryModel
     * */
    QHash<int, QByteArray> roleNames() const;    

signals:

public slots:
    void updateModel();
    void actual_patients_updateModel();
    void first_day_letalnost_updateModel(QString date);
    void first_day_perevod_updateModel(QString date);
    int getId(int row);
    QVariantMap get(int row_number) const;    
};

#endif // LISTMODEL_H
