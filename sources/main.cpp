#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QThread>

#include "database.h"
#include "listmodel.h"
#include "invasionlist.h"
#include "movementlist.h"
#include "reporter.h"
#include "comboboxsql.h"
#include "paramstorage.h"

/*!
 * \brief myMessageHandler Переопределение функции вывода отладочных сообщений сообщений.
 * \param type - тип сообщения
 * \param msg - сообщение
 */
void myMessageHandler(QtMsgType type, const QMessageLogContext &, const QString &msg);
/// Файл с отладочным выводом
QFile logFile;

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    bool db_conn = false;

    /// Конфигурация файла с отладочной информацией:
    QFileInfo fi(qApp->applicationFilePath());
    logFile.setFileName(QString("%1.log").arg(fi.baseName()));
    logFile.open(QIODevice::WriteOnly | QIODevice::Text);

    /// Устанавливаю обработчик вывода отладочной информации:
    qInstallMessageHandler(myMessageHandler);

    /// Читаю параметры из файла
    ParamStorage::getInstance()->loadParams(PARAM_FILE_NAME);

    /// Подключаемся к базе данных
    DataBase database;
    database.connectToDataBase();
    db_conn = database.get_connection_to_db_done();
    if (true != db_conn)
        qCritical() << "Database conncetion failed";

    /// Объявляем и инициализируем модель данных
    invasionlist *invas_model = new invasionlist();
    MovementList *move_model = new MovementList();
    ListModel *model = new ListModel();

    /// Объявляю и инициализирую генератор отчетов
    reporter *rep = new reporter();
    if (!(rep->set_data_source(model)))
        qCritical() << "Reporter connection to database failed";

    /// Инициализатор для полей комбобоксов
    QStringListModel model_outcome, model_storages, model_invasions, model_invtypes;
    ComboBoxSQL *combo = new ComboBoxSQL;    
    combo->FormComboBox(&model_outcome, TABLENAME_OUTCOME, TABLE_OUTCOME_JOIN);
    combo->FormComboBox(&model_invasions, TABLENAME_KORONAROGR, TABLE_KORONAROGR_NAME);
    combo->FormComboBox(&model_storages, TABLE_STORAGES_NAME, STO_STORAGES_NAME);
    combo->FormComboBox(&model_invtypes, TABLE_INVASIONTYPES_NAME, TABLE_INVASIONTYPES_FNAME);

    /// Обеспечиваем доступ к модели и классу для работы с базой данных из QML
    engine.rootContext()->setContextProperty("myModel", model);
    engine.rootContext()->setContextProperty("invasionModel", invas_model);
    engine.rootContext()->setContextProperty("movementModel", move_model);
    engine.rootContext()->setContextProperty("database", &database);
    ParamStorage *ps = ParamStorage::getInstance();
    engine.rootContext()->setContextProperty("paramstorage", ps);

    /// Путь к исполняемум файлу для заполнения полей в настройках по-умолчанию
    engine.rootContext()->setContextProperty("applicationDirPath", QGuiApplication::applicationDirPath());    

    /// Свойство для досупа к экземпляру класса репортер
    engine.rootContext()->setContextProperty("myReporter", rep);

    /// Свойства доступа к моделям данных для комбобоксов
    engine.rootContext()->setContextProperty("model_outcome", &model_outcome);
    engine.rootContext()->setContextProperty("model_invasions", &model_invasions);
    engine.rootContext()->setContextProperty("model_storages", &model_storages);
    engine.rootContext()->setContextProperty("model_invtypes", &model_invtypes);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}

/*!
 * \brief myMessageHandler Переопределение функции вывода отладочных сообщений сообщений.
 * \param type - тип сообщения
 * \param msg - сообщение
 */
void myMessageHandler(QtMsgType type, const QMessageLogContext &, const QString &msg)
{
    QString time = QString("%1.%2 %3:").arg(QDateTime::currentDateTime().time().toString())
                   .arg(QDateTime::currentDateTime().time().msec(), 3, 10, QChar('0'))
                   .arg(QString::number((unsigned int)QThread::currentThreadId(),16));

    QTextStream out(stderr);

    QString str;

    switch (type) {
        case QtInfoMsg:
            str = QString::fromUtf8("%1 Info: %2\n").arg(time.toLocal8Bit().data()).arg(msg);
            break;
        case QtDebugMsg:
            str = QString::fromUtf8("%1 Debug: %2\n").arg(time.toLocal8Bit().data()).arg(msg);
            break;
        case QtWarningMsg:
            str = QString::fromUtf8("%1 Warning: %2\n").arg(time.toLocal8Bit().data()).arg(msg);
            break;
        case QtCriticalMsg:
            str = QString::fromUtf8("%1 Critical: %2\n").arg(time.toLocal8Bit().data()).arg(msg);
            break;
        case QtFatalMsg:
            str = QString::fromUtf8("%1 Fatal: %2\n").arg(time.toLocal8Bit().data()).arg(msg);
            abort();
    }

    out << str;

    logFile.write(str.toLocal8Bit());
    logFile.flush();
}
