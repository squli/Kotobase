#ifndef COMBOBOXSQL_H
#define COMBOBOXSQL_H

#include <QObject>
#include <QStringListModel>

class ComboBoxSQL : public QObject
{
    Q_OBJECT
public:
    explicit ComboBoxSQL(QObject *parent = 0);
    bool FormComboBox(QStringListModel *model, QString table_name, QString column_name);

signals:

public slots:
};

#endif // COMBOBOXSQL_H
