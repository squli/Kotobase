#include "reporter.h"
#include <QTextDocument>
#include <QtPrintSupport/QPrinter>
#include <QSqlRecord>
#include <QSqlTableModel>
#include <QSqlQueryModel>
#include <QSqlError>

#include <QDebug>

#include <db_consts.h>
#include "movementlist.h"
#include "listmodel.h"
#include "invasionlist.h"

reporter::reporter(QObject *parent) : QObject(parent)
{

}

/*!
 * \brief reporter::set_data_source
 * \param model
 * \return
 */
bool reporter::set_data_source(ListModel *model)
{
    if (model != NULL)
    {
        data_source = model;
        return true;
    }
    return false;
}

bool reporter::prepare_report(QDate day_to_report)
{    
    QString day_report_str = day_to_report.toString("dd.MM.yyyy");
    QString day_report_str_begin = day_to_report.addDays(-1).toString("dd.MM.yyyy");
    qDebug() << day_report_str;

    data_source->actual_patients_updateModel();
    data_of_this_report.fill_zeroes();
    if (data_source->lastError().isValid())
        qDebug() << data_source->lastError();

    data_source->setHeaderData(0, Qt::Horizontal, tr(TABLE_ID));
    data_source->setHeaderData(6, Qt::Horizontal, tr(TABLE_HOSPITALDATE));
    data_source->setHeaderData(7, Qt::Horizontal, tr(TABLE_DIAGNOZ_OKS));

    for (auto i = 0; i < data_source->rowCount(); ++i)
    {
        /// Отделение/Прит
        qDebug() << data_source->record(i).value(0).toInt();
        MovementList current_move;
        current_move.getLastStorage(data_source->record(i).value(TABLE_ID).toInt());
        QString storage = current_move.record(0).value(STO_STORAGES_NAME).toString();
        if (storage == "ПРИТ")
            data_of_this_report.total_clients_count_prit += 1;
        else if (storage == "Отделение")
            data_of_this_report.total_clients_count_otdel += 1;
        qDebug() << storage;

        /// Из них непрофильные
        if (data_source->record(i).value(TABLE_DIAGNOZ_OKS) != "true")
            data_of_this_report.total_count_of_unprofile_clients += 1;

        /// Поступило с диагнозом ОКС
        qDebug() << data_source->record(i).value(TABLE_HOSPITALDATE).toString();
        if (data_source->record(i).value(TABLE_DIAGNOZ_OKS).toString() == "true" &&
           data_source->record(i).value(TABLE_HOSPITALDATE).toString() ==  day_report_str)
        {
            data_of_this_report.add_new_with_oks_clients += 1;
            if (storage == "ПРИТ")
                data_of_this_report.new_clients_to_prit += 1;
            else if (storage == "Отделение")
                data_of_this_report.new_clients_to_otdel += 1;
        }
   }

    invasionlist current_procedures;
    current_procedures.get_all_by_date(day_report_str_begin, day_report_str, "07:00");
    for (auto j = 0; j < current_procedures.rowCount(); ++j)
    {
        qDebug() << current_procedures.record(j).value(2).toInt() << "//" << current_procedures.record(j).value(3).toInt();

        //КАГ
        if ((current_procedures.record(j).value(2).toInt() == 1) && (current_procedures.record(j).value(3).toInt() == 1))
            data_of_this_report.count_of_KAG.plan += 1; //КАГ + Квота
        else if ((current_procedures.record(j).value(2).toInt() == 1) && (current_procedures.record(j).value(3).toInt() == 2))
            data_of_this_report.count_of_KAG.later += 1; //КАГ + Отсроченная
        else if ((current_procedures.record(j).value(2).toInt() == 1) && (current_procedures.record(j).value(3).toInt() == 3))
            data_of_this_report.count_of_KAG.emergency += 1; //КАГ + экстренная

        if ((current_procedures.record(j).value(2).toInt() == 2) && (current_procedures.record(j).value(3).toInt() == 1))
            data_of_this_report.count_of_STENTIR.plan += 1; //КАГ стентирование + Квота
        else if ((current_procedures.record(j).value(2).toInt() == 2) && (current_procedures.record(j).value(3).toInt() == 2))
            data_of_this_report.count_of_STENTIR.later += 1; //КАГ стентирование + Отсроченная
        else if ((current_procedures.record(j).value(2).toInt() == 2) && (current_procedures.record(j).value(3).toInt() == 3))
            data_of_this_report.count_of_STENTIR.emergency += 1; //КАГ стентирование + экстренная

        if ((current_procedures.record(j).value(2).toInt() == 3) && (current_procedures.record(j).value(3).toInt() == 1))
            data_of_this_report.count_of_BALLON.plan += 1; //КАГ баллон + Квота
        else if ((current_procedures.record(j).value(2).toInt() == 3) && (current_procedures.record(j).value(3).toInt() == 2))
            data_of_this_report.count_of_BALLON.later += 1; //КАГ баллон + Отсроченная
        else if ((current_procedures.record(j).value(2).toInt() == 3) && (current_procedures.record(j).value(3).toInt() == 3))
            data_of_this_report.count_of_BALLON.emergency += 1; //КАГ баллон + экстренная

        //Тромболизис
        if (current_procedures.record(j).value(2).toInt() == 6)
            data_of_this_report.count_of_tromboliz += 1;

        //Не выполнено инвазивное вмешательство
        if ((current_procedures.record(j).value(2).toInt() == 4) && (current_procedures.record(j).value(3).toInt() == 4))
            data_of_this_report.count_of_OTSUTSVIE_POKAZANIY += 1; //Не выполнено + нет эскренных показаний
        else if ((current_procedures.record(j).value(2).toInt() == 4) && (current_procedures.record(j).value(3).toInt() == 5))
            data_of_this_report.count_of_OTKAZ_CLIENT += 1; //Не выполнено + отказ больного
        else if ((current_procedures.record(j).value(2).toInt() == 4) && (current_procedures.record(j).value(3).toInt() == 6))
            data_of_this_report.count_of_PROTIVOPOKAZANIY += 1; //Не выполнено + наличие противопоказаний

        data_of_this_report.count_of_not_invasion = data_of_this_report.count_of_OTSUTSVIE_POKAZANIY +
                                                    data_of_this_report.count_of_OTKAZ_CLIENT +
                                                    data_of_this_report.count_of_PROTIVOPOKAZANIY;


    }

    /// СМЕРТЬ в течение последних суток
    ListModel lm;
    lm.first_day_letalnost_updateModel(day_report_str);
    for (auto k = 0; k < lm.rowCount(); ++k)
    {
        data_of_this_report.count_of_LETALNOST += 1;
        letality_typedef letal;
        letal.fam = lm.record(k).value(1).toString();
        letal.nam = lm.record(k).value(2).toString();
        if (lm.record(k).value(7) == true)
            letal.dgz = "ОКС";
        else
            letal.dgz = "не профильный";
        letal.outcome_date = lm.record(k).value(9).toString();
        data_of_this_report.ListOfLetalnost.append(letal);
    }

    /// ПЕРЕВОД в течение последних суток
    ListModel lmm;
    lmm.first_day_perevod_updateModel(day_report_str);
    for (auto k = 0; k < lmm.rowCount(); ++k)
    {
        if (lmm.record(k).value(7) == true)
            data_of_this_report.count_of_perevod_another_otdels += 1;
    }

    return true;
}

/*!
 * \brief reporter::make_report
 * \return
 */
bool reporter::make_report(QString date_report_str)
{
    //QDate date_of_report = QDate::currentDate();
    QDate date_of_report = QDate::fromString(date_report_str, "dd.MM.yyyy");
    if (!prepare_report(date_of_report))
        qDebug() << "[ERR] prepare_report() failed";

    QString letalnost_table = "";
    //формирование списка досуточной летальности
    if (!data_of_this_report.ListOfLetalnost.empty())
    {
        for(QList<letality_typedef>::Iterator it = data_of_this_report.ListOfLetalnost.begin(); it != data_of_this_report.ListOfLetalnost.end(); it++)
        {
            letalnost_table.append("<table><tr>   \
             <td width=\"80px\">");letalnost_table.append(it->fam + QString("</td>   \
             <td width=\"80px\">"));letalnost_table.append(it->nam + QString("</td>   \
             <td width=\"80px\">"));letalnost_table.append(it->dgz + QString("</td>   \
            <td width=\"80px\">"));letalnost_table.append(it->outcome_date + QString("</td>   \
            </tr></table>"));
        }
    }

    // форомирование страницы с данными
    QTextDocument document;
    document.setHtml("<h1>РСЦ: ОКС за " + QString(date_of_report.toString("dd.MM.yyyy"))  + "</h1><br>         \
                      <table width=\"400px\"        \
                             <tr>                   \
                               <td width=\"200px\"><h2>Отделение/ПРИТ</h2></td> \
                               <td width=\"200px\"><h2>" + QString("%1").arg(data_of_this_report.total_clients_count_otdel) + "/" + QString("%1").arg(data_of_this_report.total_clients_count_prit) + "</h2></td>  \
                             </tr>  \
                             <tr>   \
                               <td width=\"200px\"><h2>Из них непрофильные</h2></td>    \
                               <td width=\"200px\"><h2>" + QString("%1").arg(data_of_this_report.total_count_of_unprofile_clients) + "</h2></td>    \
                             </tr>  \
                             <tr>   \
                              <td width=\"200px\"><h2>Поступило с диагнозом ОКС</h2></td>   \
                              <td width=\"200px\"><h2>" + QString("%1").arg(data_of_this_report.add_new_with_oks_clients) + "</h2></td> \
                             </tr>  \
                             <tr>   \
                              <td width=\"200px\"><h2><i>Из них:</i></h2></td>   \
                              <td width=\"200px\"></td> \
                             </tr>  \
                             <tr>   \
                              <td width=\"200px\"><h2>Госпитализировано в ПРИТ</h2></td>   \
                              <td width=\"200px\"><h2>" + QString("%1").arg(data_of_this_report.new_clients_to_prit) + "</h2></td> \
                             </tr>  \
                             <tr>   \
                              <td width=\"200px\"><h2>Госпитализировано в РСЦ</h2></td>   \
                              <td width=\"200px\"><h2>" + QString("%1").arg(data_of_this_report.new_clients_to_otdel) + "</h2></td> \
                             </tr>  \
                             <tr>   \
                              <td width=\"200px\"><h2>На другие отделения ОРИТ</h2></td>   \
                              <td width=\"200px\"><h2>" + QString("%1").arg(data_of_this_report.count_of_perevod_another_otdels) + "</h2></td> \
                             </tr>  \
                             <tr>   \
                              <td width=\"200px\"><h2>Диагноз снят по результатам обследования</h2></td>   \
                              <td width=\"200px\"><h2>" + QString("%1").arg(data_of_this_report.new_clients_to_otdel) + "</h2></td> \
                             </tr>  \
                            <tr>   \
                             <td width=\"200px\"><h2>Проведено КАГ план/отсроч/экстр</h2></td>   \
                             <td width=\"200px\"><h2>" + QString("%1").arg(data_of_this_report.count_of_KAG.plan) + "/" + QString("%1").arg(data_of_this_report.count_of_KAG.later) + "/" + QString("%1").arg(data_of_this_report.count_of_KAG.emergency) + "</h2></td> \
                            </tr>  \
                            <tr>   \
                             <td width=\"200px\"><h2>Стентирование план/отсроч/экстр</h2></td>   \
                             <td width=\"200px\"><h2>" + QString("%1").arg(data_of_this_report.count_of_STENTIR.plan) + "/" + QString("%1").arg(data_of_this_report.count_of_STENTIR.later) + "/" + QString("%1").arg(data_of_this_report.count_of_STENTIR.emergency) + "</h2></td> \
                            </tr>  \
                            <tr>   \
                             <td width=\"200px\"><h2>Баллонная ангинопластика план/отсроч/экстр</h2></td>   \
                             <td width=\"200px\"><h2>" + QString("%1").arg(data_of_this_report.count_of_BALLON.plan) + "/" + QString("%1").arg(data_of_this_report.count_of_BALLON.later) + "/" + QString("%1").arg(data_of_this_report.count_of_BALLON.emergency) + "</h2></td> \
                            </tr>  \
                            <tr>   \
                             <td width=\"200px\"><h2>Не выполнено инвазивное вмешательство</h2></td>   \
                             <td width=\"200px\"><h2>" + QString("%1").arg(data_of_this_report.count_of_not_invasion) + "</h2></td> \
                            </tr>  \
                            <tr>   \
                             <td width=\"200px\"><h2>Проведено в/в ТРОМБОЛИЗИС</h2></td>   \
                             <td width=\"200px\"><h2>" + QString("%1").arg(data_of_this_report.count_of_tromboliz) + "</h2></td> \
                            </tr>  \
                            </tr>  \
                            <tr>   \
                             <td width=\"200px\"><h2><i>По причине:</i></h2></td>   \
                             <td width=\"200px\"><h2>""</h2></td> \
                            </tr>  \
                            </tr>  \
                            <tr>   \
                             <td width=\"200px\"><h2><i>Отсутствие экстренных показаний</i></h2></td>   \
                             <td width=\"200px\"><h2>" + QString("%1").arg(data_of_this_report.count_of_OTSUTSVIE_POKAZANIY ) + "</h2></td> \
                            </tr>  \
                            </tr>  \
                            <tr>   \
                             <td width=\"200px\"><h2><i>Отказ больного</i></h2></td>   \
                             <td width=\"200px\"><h2>" + QString("%1").arg(data_of_this_report.count_of_OTKAZ_CLIENT) + "</h2></td> \
                            </tr>  \
                            </tr>  \
                            <tr>   \
                             <td width=\"200px\"><h2><i>Наличие противопоказаний</i></h2></td>   \
                             <td width=\"200px\"><h2>" + QString("%1").arg(data_of_this_report.count_of_PROTIVOPOKAZANIY) + "</h2></td> \
                            </tr>  \
                            <tr>   \
                             <td width=\"200px\"><h2><i>Досуточная летальность:</i></h2></td>   \
                             <td width=\"200px\"><h2>" + QString("%1").arg(data_of_this_report.count_of_LETALNOST) + "</h2></td> \
                            </tr></table>"
                                + letalnost_table);


    QPrinter *printer = new QPrinter(QPrinter::HighResolution);
    printer->setPageSize(QPrinter::A4);
    printer->setOutputFormat(QPrinter::PdfFormat);

    printer->setOutputFileName(QString(date_of_report.toString("yyyy_MM_dd") + "_report.pdf"));
    document.print(printer);
    return true;
}
