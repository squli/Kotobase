import QtQuick 2.0
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.4

Item {    
    width: 640
    height: 480

    Text {
        id:header
        font.pointSize: 10
        text: "Настройки программы"
        anchors.top: parent.top
        anchors.topMargin: 20
    }

    GridLayout{
        anchors.top: header.bottom
        anchors.topMargin: 20
        anchors.left: parent.left
        anchors.leftMargin: 20
        columnSpacing: 10
        columns: 2
        rows:4

        Text {
            id:r1t1
            width: 200
            font.pointSize: 10
            text: "Путь к базе данных"
            Layout.column: 1
            Layout.row: 1
        }

        TextEdit {
            id:r1te1
            Layout.column: 2
            Layout.row: 1
            text: paramstorage.getValueByEnum(0)
        }

        Text {
            id:db_filename_title
            width: 200
            font.pointSize: 10
            text: "Имя файла базы данных"
            Layout.column: 1
            Layout.row: 2
        }

        TextEdit {
            id:db_filename
            Layout.column: 2
            Layout.row: 2
            text: paramstorage.getValueByEnum(1)
        }

        Text {
            id:db_dbname_title
            width: 200
            font.pointSize: 10
            text: "Имя базы данных"
            Layout.column: 1
            Layout.row: 3
        }

        TextEdit {
            id:db_dbname
            Layout.column: 2
            Layout.row: 3
            text: paramstorage.getValueByEnum(2)
        }

        Text {
            id:r2t1
            width: 200
            font.pointSize: 10
            text: "Путь к папке с отчетами"
            Layout.column: 1
            Layout.row: 4
        }

        TextEdit {
            id:r2te1
            text: paramstorage.getValueByEnum(3)
            Layout.column: 2
            Layout.row: 4
        }

        Text {
            id:time_to_begin_work_title
            width: 200
            font.pointSize: 10
            text: "Время начала смены\n для формирования отчетов"
            Layout.column: 1
            Layout.row: 5
        }

        TextEdit {
            id:time_to_begin_work
            text:paramstorage.getValueByEnum(4)
            Layout.column: 2
            Layout.row: 5
        }

        Text {
            id:time_to_finish_work_title
            width: 200
            font.pointSize: 10
            text: "Время окончания смены\n для формирования отчетов"
            Layout.column: 1
            Layout.row: 6
        }

        TextEdit {
            id:time_to_finish_work
            text:paramstorage.getValueByEnum(5)
            Layout.column: 2
            Layout.row: 6
        }

        Button {
            id: b1
            text: qsTr("Сохранить")
            Layout.column: 1
            Layout.row: 8
            onClicked: {
                var new_params = {};

                new_params.db_path_name = r1te1.text.toString()
                new_params.db_file_name = db_filename.text.toString()
                new_params.db_host_name = db_dbname.text.toString()
                new_params.rep_path_name = r2te1.text.toString()
                new_params.rep_time_start = time_to_finish_work_title.toString()
                new_params.rep_time_stop = time_to_finish_work_title.toString()

                paramstorage.updateFromGui(new_params);

                database.closeDataBase()
                database.connectToDataBase()
                conn_db_stat.set()

                controlsDialog.close()
            }
        }

        Button {
            id: b2
            text: qsTr("Отмена")
            Layout.column: 2
            Layout.row: 8
            onClicked: {

                controlsDialog.close()
            }
        }
    }

}

