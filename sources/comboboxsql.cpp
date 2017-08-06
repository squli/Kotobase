#include "comboboxsql.h"
#include <QSqlQueryModel>
#include <QSqlRecord>
#include <QVariant>

ComboBoxSQL::ComboBoxSQL(QObject *parent) : QObject(parent)
{

}

bool ComboBoxSQL::FormComboBox(QStringListModel *model, QString table_name, QString column_name)
{
    int i = 0;
    QStringList *l = new QStringList;
    QSqlQueryModel *m = new QSqlQueryModel;
    m->setQuery("SELECT * FROM " + table_name);

    for (i = 0; i < m->rowCount(); ++i)
    {
        QSqlRecord r;
        r = m->record(i);
        l->append(r.value(column_name).toString());
    }

    model->setStringList(*l);

    if (i != 0)
        return true;
    else
        return false;
}
