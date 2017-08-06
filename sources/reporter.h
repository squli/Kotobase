#ifndef REPORTER_H
#define REPORTER_H

#include <QObject>
#include <QDate>
#include "listmodel.h"
#include <QList>

typedef struct {
    int plan = 0;
    int later = 0;
    int emergency = 0;
} type_of_invasions_typedef;

typedef struct {
    QString fam;
    QString nam;
    QString dgz;
    QString outcome_date;
} letality_typedef;

/// Структура для хранения данных представляемых в отчете
struct report_data{
    int total_clients_count_otdel;
    int total_clients_count_prit;
    int add_new_with_oks_clients;
    int total_count_of_unprofile_clients;

    int new_clients_to_prit;
    int new_clients_to_otdel;
    int count_of_perevod_another_otdels;

    type_of_invasions_typedef count_of_KAG;
    type_of_invasions_typedef count_of_STENTIR;
    type_of_invasions_typedef count_of_BALLON;

    int count_of_tromboliz;
    int count_of_not_invasion;

    int count_of_OTSUTSVIE_POKAZANIY;
    int count_of_OTKAZ_CLIENT;
    int count_of_PROTIVOPOKAZANIY;
    int count_of_LETALNOST;
    QList<letality_typedef>ListOfLetalnost;

    report_data() :
                total_clients_count_otdel(0),
                total_clients_count_prit(0),
                add_new_with_oks_clients(0),
                total_count_of_unprofile_clients(0),

                new_clients_to_prit(0),
                new_clients_to_otdel(0),
                count_of_perevod_another_otdels(0),

                count_of_tromboliz(0),
                count_of_not_invasion(0),
                count_of_OTSUTSVIE_POKAZANIY(0),
                count_of_OTKAZ_CLIENT(0),
                count_of_PROTIVOPOKAZANIY(0),
                count_of_LETALNOST(0)
    { }

    void fill_zeroes(void)
    {
        total_clients_count_otdel = 0;
        total_clients_count_prit = 0;
        add_new_with_oks_clients = 0;
        total_count_of_unprofile_clients = 0;

        new_clients_to_prit = 0;
        new_clients_to_otdel = 0;
        count_of_perevod_another_otdels = 0;

        count_of_KAG.emergency = 0;
        count_of_KAG.later = 0;
        count_of_KAG.plan = 0;
        count_of_STENTIR.emergency = 0;
        count_of_STENTIR.later = 0;
        count_of_STENTIR.plan = 0;
        count_of_BALLON.emergency = 0;
        count_of_BALLON.later = 0;
        count_of_BALLON.plan = 0;

        count_of_not_invasion = 0;
        count_of_tromboliz = 0;
        count_of_OTSUTSVIE_POKAZANIY = 0;
        count_of_OTKAZ_CLIENT = 0;
        count_of_PROTIVOPOKAZANIY = 0;
        count_of_LETALNOST = 0;
        ListOfLetalnost.clear();
    }
};

class reporter : public QObject
{
    Q_OBJECT
public:
    explicit reporter(QObject *parent = 0);
    /// Инициализирует локальный указатель на модель данных
    bool set_data_source(ListModel *model);

private:
    /// Модель с данными о текущих пациентах
    ListModel *data_source;    
    /// Структура для хранения данных
    report_data data_of_this_report;
    /// Метод формирования структуры с данными для отчета
    bool prepare_report(QDate day_to_report);

signals:

public slots:
    /// Создание PDF с отображением данных из data_of_this_report
    //bool make_report();
    bool make_report(QString date_report_str);
};

#endif // REPORTER_H
