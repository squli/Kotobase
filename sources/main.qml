import QtQuick 2.3
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.2
import QtQuick.Window 2.0
import QtQuick.Layouts 1.1
import QtQuick.Controls.Styles 1.4

ApplicationWindow {
    id: root
    visible: true
    title: "Котобаза"
    color:"white"
    maximumHeight: 768
    maximumWidth: 1024
    minimumHeight: 768
    minimumWidth: 1024

    // Глобальные переменные
    Item {
        id:global_vars

        /// Флаг предназначен для выбора глобальной переменной,
        /// в которой будет сохранено значение даты, выбранное в окне календаря
        // 1 - для dateHospital
        // 2 - для dateInvasion
        // 3 - для dateMovement
        property int date_flag: 0;
        // функция для установки значений
        function set_date_flag(val)
        {
            date_flag = val
        }

        ///переменные для сохранения дат при вызове окна календаря
        property var dateHospital: new Date();
        property var dateOutcome: new Date();
        property var dateInvasion: new Date();
        property var dateMovement: new Date();
        property var dateReport: new Date();
    }


    /*!
       \brief Окно отображения выбора даты на календаре
      */
    Dialog {
        id: dialogCalendar        
        width: 250
        height: 300

        // Создаем контент диалогового окна
        contentItem: Rectangle {
            id: dialogRect
            color: "#f7f7f7"

            // Первым идёт кастомный календарь
            Calendar {
                id: calendar
                // Размещаем его в верхней части диалога и растягиваем по ширине
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: row.top

            }

            // Делаем панель с кнопками
            Row {
                id: row
                height: 48
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: parent.bottom

                // Кнопка для закрытия диалога
                Button {
                    id: dialogButtonCalCancel
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    width: parent.width / 2 - 1
                    text: qsTr("Cancel")

                    // По нажатию на кнопку - просто закрываем диалог
                    onClicked: dialogCalendar.close()
                }

                // Вертикальный разделитель между кнопками
                Rectangle {
                    id: dividerVertical_hospital
                    width: 2
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    color: "#d7d7d7"
                }

                // Кнопка подтверждения выбранной даты
                Button {
                    id: dialogButtonCalOk_hospital
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    width: parent.width / 2 - 1
                    text: qsTr("Ok")

                    /* По клику по кнопке сохраняем выбранную дату во временную переменную
                     * и помещаем эту дату на кнопку в главном окне,
                     * после чего закрываем диалог
                     */
                    onClicked: {
                        if (global_vars.date_flag === 1) {
                            global_vars.dateHospital = calendar.selectedDate
                            date_hospital.text = Qt.formatDate(global_vars.dateHospital, "dd.MM.yyyy");
                            global_vars.set_date_flag(0)
                        }
                        if (global_vars.date_flag === 2)
                        {
                            global_vars.dateInvasion = calendar.selectedDate
                            add_inv_win.set_date(Qt.formatDate(global_vars.dateInvasion, "dd.MM.yyyy"));
                            global_vars.set_date_flag(0)
                        }
                        if (global_vars.date_flag === 3)
                        {
                            global_vars.dateMovement = calendar.selectedDate
                            add_mov_win.set_date(Qt.formatDate(global_vars.dateMovement, "dd.MM.yyyy"));
                            global_vars.set_date_flag(0)
                        }
                        if (global_vars.date_flag === 4)
                        {
                            global_vars.dateOutcome = calendar.selectedDate
                            date_outcome.text = Qt.formatDate(global_vars.dateOutcome, "dd.MM.yyyy");
                            global_vars.set_date_flag(0)
                        }                        
                        if (global_vars.date_flag === 5)
                        {
                            global_vars.dateReport = calendar.selectedDate
                            date_report.text = "Дата отчета:" + Qt.formatDate(global_vars.dateReport, "dd.MM.yyyy");
                            global_vars.set_date_flag(0)
                        }

                        dialogCalendar.close();
                    }
                }
            }
        }

        /* Данная функция необходима для того, чтобы
         * установить дату с кнопки в календарь,
         * иначе календарь откроется с текущей датой
         */
        function show(x){
            calendar.selectedDate = x
            dialogCalendar.open()
        }
    }
    // /календарь


    /*!
       \brief Окно отображения диалога настроек
      */
    Window {
        id: controlsDialog
        title: "Настройки"
        width: 640
        height: 480
        maximumHeight: 480
        minimumHeight: 480
        maximumWidth:  640
        minimumWidth:  640

        ConfWindow {}
    }
    // /настройки

    /*!
       \brief Окно отображения диалога справки
      */
    Window {
        id: helpDialog
        title: "Справка"
        width: 640
        height: 500


        HelpWindow {}

    }
    // /справка

    /*!
       \brief Главное окно
      */
    Rectangle {
        width: 1024
        height: 768
        color:"white"

        /*Image {
            source: "fon.png"
        }*/

        ///первый ряд - текущая дата и состояние соединения с базой данных
        Row {
            id: row1
            spacing: 50
            anchors.left: parent.left // прижимаем левую часть рамки к левой стороне окна
            anchors.right: parent.right // а правую сторону к правой
            anchors.topMargin: 10
            anchors.leftMargin: 10
            anchors.rightMargin: 2

            Text {
                id:currenDateTime
                font.pointSize: 12
                function set() {
                    currenDateTime.text = "Сейчас " + new Date().toLocaleString(Qt.locale("ru_RU"))
                }
            }

            Timer {
                id: textTimer
                interval: 1000
                repeat: true
                running: true
                triggeredOnStart: true
                onTriggered: {
                    currenDateTime.set()
                    conn_db_stat.set()
                }
            }

            Text {
                id: conn_db_stat
                font.pointSize: 12
                function set() {
                    if (database.get_connection_to_db_done() === true)
                        conn_db_stat.text = " - cоединение с базой данных установлено"
                    else
                        conn_db_stat.text = " - cоединение с базой данных НЕ установлено"
                }
            }
        }

        ///второй ряд - меню слева и форма отображения данных о пациенте справа
        GridLayout {
            id: row2
            anchors.top: row1.bottom
            anchors.left: parent.left // прижимаем левую часть рамки к левой стороне окна
            anchors.topMargin: 10
            anchors.leftMargin: 10
            rows: 2
            columns: 1
            height: 310

            Row {
                id:nav_bar
                Layout.row: 1
                spacing: 10
                anchors.top: parent.top // прижимаем левую часть рамки к левой стороне окна
                anchors.topMargin: 2
                anchors.left: parent.left
                anchors.leftMargin: 1

                Button {
                    text: qsTr("Просмотр базы")
                    width: 120

                    //открыть окно просмотра базы данных и обновить модель данных
                    onClicked: {
                        add_client_button.current_regime = 0
                        //if (!row3_tableView.visible)
                        //{
                            tableView.update()
                            row3_tableView.visible = true
                        //}
                        //else
                        //{
                           // row3_tableView.visible = false
                            add_inv_button.visible = false
                            add_mov_button.visible = false
                        //}
                    }
                }

                Button {
                    text: qsTr("Пациенты сейчас")
                    width: 120

                    //открыть окно просмотра базы данных и показать текущих пациентов
                    onClicked: {
                        add_client_button.current_regime = 0
                        //if (!row3_tableView.visible)
                        //{
                            tableView.actual_update()
                            row3_tableView.visible = true
                        //}
                        //else
                           // row3_tableView.visible = false
                            add_inv_button.visible = false
                            add_mov_button.visible = false
                        //
                    }
                }

                Button {
                    text: qsTr("Внести запись в базу")
                    width: 120

                    // добавить запись в базу
                    onClicked: {
                        add_client_button.current_regime = 1
                    }
                }

                //Button {
                //    text: qsTr("Найти запись в базе")
                //    width: 120
                //
                //    // найти запись
                //    onClicked: {
                //
                //    }
                //}

                Button {
                    id:date_report
                    text: qsTr("Отчет за:") + Qt.formatDate(new Date(), "dd.MM.yyyy");
                    width: 120

                    // напечатать отчет в pdf
                    onClicked: {
                        dialogCalendar.show(global_vars.dateReport)
                        global_vars.set_date_flag(5)
                    }
                }

                Button {
                    text: qsTr("Сделать отчет")
                    width: 120

                    // напечатать отчет в pdf
                    onClicked: {
                        myReporter.make_report(Qt.formatDate(global_vars.dateReport, "dd.MM.yyyy"));
                    }
                }                                

                Button {
                    text: qsTr("Настройки")
                    width: 120

                    // открыть панель настроек
                    onClicked: {
                        if (!controlsDialog.visible)
                            controlsDialog.show()
                    }
                }

                Button {
                    text: qsTr("Справка")
                    width: 120

                    //справка
                    onClicked: {
                        helpDialog.show()
                    }
                }
            }
            // /меню

            /// данные о клиенте
            Rectangle {
                id:new_client_window
                Layout.row: 2
                color: "WHITE"
                border.width: 2
                border.color: "GRAY"
                width: 1004
                height: 310
                anchors.top: nav_bar.bottom // прижимаем левую часть рамки к левой стороне окна
                anchors.topMargin: 5
                anchors.left: parent.left
                anchors.leftMargin: 1

                Text {
                    id:add_client_header
                    anchors.top: parent.top // прижимаем левую часть рамки к левой стороне окна
                    anchors.topMargin: 3
                    anchors.left: parent.left
                    anchors.leftMargin: 10
                    text:"Просмотр пациентов"
                }

                RowLayout {
                    id: patient_detail_info
                    anchors.top: add_client_header.bottom
                    anchors.topMargin: 0
                    anchors.left: parent.left
                    anchors.leftMargin:10
                    spacing: 10


                    GridLayout {
                        id: add_new_client
                        rows:7
                        columns: 2

                        Text {
                            text: qsTr("Фамилия")
                            Layout.row: 1
                            Layout.column: 1
                        }
                        TextField {
                            id: snameField
                            Layout.row: 1
                            Layout.column: 2
                            width:100
                            text: qsTr("Фамилия")
                            validator: RegExpValidator { regExp: /(^[А-Я][а-я]*)/}
                        }

                        Text {
                            text: qsTr("Имя")
                            Layout.row: 2
                            Layout.column: 1
                        }
                        TextField {
                            id: fnameField
                            width:80
                            Layout.row: 2
                            Layout.column: 2
                            validator: RegExpValidator { regExp: /(^[А-Я][а-я]*)/}
                        }

                        Text {
                            text: qsTr("Отчество")
                            Layout.row: 3
                            Layout.column: 1
                        }
                        TextField {
                            id: patField
                            width:80
                            Layout.row: 3
                            Layout.column: 2
                            validator: RegExpValidator { regExp: /(^[А-Я][а-я]*)/}
                        }


                        Row
                        {
                            Layout.row: 4
                            Layout.column: 1
                            Layout.columnSpan: 2
                            spacing: 10

                        Text {
                            text: qsTr("Возраст")
                            //Layout.row: 4
                            //Layout.column: 1
                        }
                        TextField {
                            id: ageField
                            width:25
                            //Layout.row: 4
                            //Layout.column: 2
                            validator: RegExpValidator{ regExp: /(^[0-9]{2})/ }
                        }

                        Text {
                            text: qsTr("Номер истории\n болезни")
                           // Layout.row: 5
                           // Layout.column: 1
                        }
                        TextField
                        {
                            id: history_number
                            width:50
                         //   Layout.row: 5
                         //   Layout.column: 2
                            text: "0"
                        }
                        }

                       // Text {
                       //     text: qsTr("Дата госпитализации")
                       //     Layout.row: 6
                       //     Layout.column: 1
                       // }
                       // Button {
                       //     id: date_hospital
                       //     text: Qt.formatDate(global_vars.dateHospital, "dd.MM.yyyy");
                       //     Layout.row: 6
                       //     Layout.column: 2
                       //     onClicked: {
                       //         dialogCalendar.show(global_vars.dateHospital)
                       //         global_vars.set_date_flag(1)
                       //     }
                       // }


                        Row
                        {
                            Layout.row: 6
                            Layout.column: 1
                            Layout.columnSpan: 2
                            spacing: 10

                            Text {
                                text: qsTr("Дата и время\n госпитализации")
                            }
                            Button {
                                    id: date_hospital
                                        onClicked: {
                                            dialogCalendar.show(global_vars.dateHospital)
                                            global_vars.set_date_flag(1)
                                    }
                                }
                            TextField {
                                    id: time_hospital
                                    anchors.left: date_hospital.Left
                                    anchors.leftMargin: 2
                                    width: 50
                                    validator: RegExpValidator { regExp: /(^([0,1][0-9])|(2[0-3])):([0-5]{1}[0-9]{1})/}
                                }
                        }


                        GroupBox {
                            title: "Диагноз:"
                            Layout.row: 7
                            Layout.column: 1
                            Layout.columnSpan: 2

                            RowLayout {
                                ExclusiveGroup { id: diagnoz_group }
                                RadioButton {
                                    id: oks
                                    text: "ОКС"
                                    checked: true
                                    exclusiveGroup: diagnoz_group
                                }
                                RadioButton {
                                    id:unprofile
                                    text: "Непрофильный"
                                    exclusiveGroup: diagnoz_group
                                }
                            }
                        }

                        Text {
                            text: qsTr("Исход")
                            Layout.row: 8
                            Layout.column: 1
                        }
                        ComboBox {
                            id: box_model_outcome
                            anchors.topMargin: 5
                            width: 200
                            model: model_outcome
                            textRole: "display"
                            Layout.row: 8
                            Layout.column: 2
                        }

                        Row
                        {
                            Layout.row: 9
                            Layout.column: 1
                            Layout.columnSpan: 2
                            spacing: 10

                            //anchors.top: add_new_client.bottom
                            //anchors.topMargin: 1

                            Text {
                                text: qsTr("Дата и время\n исхода")
                               // Layout.row: 9
                               // Layout.column: 1
                            }
                            Button {
                                    id: date_outcome
                                   // Layout.row: 9
                                 //   Layout.column: 2
                                   // text: Qt.formatDate(global_vars.dateOutcome, "dd.MM.yyyy");
                                        onClicked: {
                                            dialogCalendar.show(global_vars.dateOutcome)
                                            global_vars.set_date_flag(4)
                                    }
                                }
                            TextField {
                                    id: time_outcome
                                   // Layout.row: 9
                                  //  Layout.column: 2
                                    anchors.left: date_outcome.Left
                                    anchors.leftMargin: 2
                                    width: 50
                                    validator: RegExpValidator { regExp: /(^([0,1][0-9])|(2[0-3])):([0-5]{1}[0-9]{1})/}
                                }
                        }
/*
                        Text {
                            text: qsTr("Дата и время\n исхода")
                            Layout.row: 9
                            Layout.column: 1
                        }
                        Button {
                                id: date_outcome
                                Layout.row: 9
                                Layout.column: 2
                                text: Qt.formatDate(global_vars.dateOutcome, "dd.MM.yyyy");
                                    onClicked: {
                                        dialogCalendar.show(global_vars.dateOutcome)
                                        global_vars.set_date_flag(1)
                                }
                            }
                        TextField {
                                id: time_outcome
                                Layout.row: 9
                                Layout.column: 2
                                anchors.left: date_outcome.Left
                                anchors.leftMargin: 2
                                width: 30
                                validator: RegExpValidator { regExp: /(^([0,1][0-9])|(2[0-3])):([0-5]{1}[0-9]{1})/}
                            }                                               
*/
                        /*!
                            \brief Заполнение объекта qml данными из полей ввода.
                            \param new_client заполняемый объект
                          */
                        function make_object(new_client)
                        {
                            /// Формирование структуры с данными для передачи в cpp
                            new_client.surname = snameField.text.toString()
                            new_client.name = fnameField.text.toString()
                            new_client.pat = patField.text.toString()
                            new_client.age = ageField.text.toString()
                            new_client.history_num = history_number.text.toString()
                            new_client.hospitaliz_date = Qt.formatDate(global_vars.dateHospital, "yyyy-MM-dd") + " " + time_hospital.text.toString()

                            if (oks.checked === true)
                                new_client.diagnoz_oks = true
                            else if (unprofile.checked === true)
                                new_client.diagnoz_oks = false

                            new_client.outcome = box_model_outcome.currentIndex

                            if (box_model_outcome.currentIndex != 0)
                                new_client.outcome_date = Qt.formatDate(global_vars.dateOutcome, "yyyy-MM-dd") + " " + time_outcome.text.toString()

                            return new_client;
                        }

                        /*!
                            \brief Проверка вменяемости данных в новом объекте
                            \param new_client заполняемый объект
                          */
                        function check_object(new_client)
                        {
                            console.log("Check input data from new_client")
                            if (new_client.surname === "Фамилия" )
                            {
                                console.log("surname undefined - %s", new_client.surname)
                                return 1;
                            }
                            if (new_client.history_num === "0")
                            {
                                console.log("history_num is NULL: %d", new_client.surname.lenght)
                                return 2;
                            }

                            return 0;
                        }

                        MessageDialog {
                            id:add_client_error_dialog
                            title: "Ошибка"
                            text: "Ошибка при попытке добавления клиента"
                            icon: StandardIcon.Critical
                            visible: false

                            function set_error_type (err_num)
                            {
                                switch (err_num)
                                {
                                    case 1:
                                    {
                                        add_client_error_dialog.text = "Не заполнена фамилия";
                                        break;
                                    }
                                    case 2:
                                    {
                                        add_client_error_dialog.text = "Не заполнен номер истории болезни";
                                        break;
                                    }
                                    case 3:
                                    {
                                        add_client_error_dialog.text = "Ошибка при заполнении пребывания в ПРИТ";
                                        break;
                                    }
                                    case 4:
                                    {
                                        add_client_error_dialog.text = "Ошибка при заполнении пребывания на отдлении";
                                        break;
                                    }
                                }
                            }
                        }

                        Button {
                            id:add_client_button
                            visible:false
                            text: qsTr("Добавить клиента")
                            Layout.row: 10
                            Layout.column: 1
                            Layout.columnSpan: 2

                            // текущий режим работы:
                            //  0 - добавление записей
                            //  1 - редактирование записи
                            //  2 - поиск записи
                            property int current_regime: 0

                            onCurrent_regimeChanged:
                            {
                                /* при изменении режима работы кнопки:
                                    отобразисть заголовок и сменить название кнопки
                                */
                                console.log("onCurrent_regimeChanged:", current_regime)

                                if (current_regime == 0)
                                {
                                    add_client_header.text = "Просмотр пациентов"
                                    add_client_button.visible = false
                                    movement_column.visible = true
                                    invasions_column.visible = true
                                }

                                if (current_regime == 1)
                                {
                                    add_client_header.text = "Добавление нового пациента"
                                    add_client_button.text = qsTr("Добавить клиента")
                                    add_client_button.visible = true
                                    movement_column.visible = false
                                    invasions_column.visible = false

                                }

                                if (current_regime == 2)
                                {
                                    add_client_header.text = "Редактирование пациента"
                                    add_client_button.text = qsTr("Отредактировать запись")
                                    add_client_button.visible = true
                                    movement_column.visible = true
                                    invasions_column.visible = true
                                }

                                if (current_regime == 3)
                                {
                                    add_client_header.text = "Поиск пациента"
                                    add_client_button.text = qsTr("Найти записи")
                                    add_client_button.visible = true
                                }
                            }

                            onClicked: {
                                if (current_regime != 3)    //если нужно отправлять поля в базу: 1, 2
                                {
                                    var new_client = {};
                                    add_new_client.make_object(new_client);
                                    var result_of_check = add_new_client.check_object(new_client);

                                    if (result_of_check === 0)
                                    {
                                        if (add_client_button.current_regime === 1)
                                            database.inserIntoTablePatients(new_client);
                                        else if (add_client_button.current_regime === 2)
                                             database.updateRecord(myModel.getId(tableView.currentRow), new_client);
                                        else
                                        {
                                            console.log("Некорректное значение свойства current_regime")
                                        }

                                         tableView.actual_update(); // И обновляем модель данных с новой записью
                                        add_client_button.visible = false;
                                    }
                                    else
                                    {
                                        //show error Window
                                        add_client_error_dialog.set_error_type(result_of_check)
                                        add_client_error_dialog.open()
                                    }
                                }
                                else
                                {
                                    //поиск
                                }
                            }
                    }
                }




                Rectangle {
                    id: movement_column
                    anchors.top: parent.top
                    anchors.topMargin: 0                   
                    width:280
                    height:250
                    color: "GRAY"

                    TableView{
                        id:movement_tableview
                        width:280
                        height:250

                        TableViewColumn {
                            id:new_storage
                            role: "new_storage"
                            title: "Находится"
                        }
                        TableViewColumn {
                            id:storage_datetime
                            role: "movement_datetime"
                            title: "Поступил"
                        }
                        model:movementModel

                        // Настройка строки в TableView для перехавата левого клика мыши
                        rowDelegate: Rectangle {
                            anchors.fill: parent
                            color: styleData.selected ? 'skyblue' : (styleData.alternate ? 'whitesmoke' : 'white');
                            MouseArea {
                                anchors.fill: parent
                                acceptedButtons: Qt.RightButton | Qt.LeftButton
                                onClicked: {
                                    movement_tableview.selection.clear()
                                    movement_tableview.selection.select(styleData.row)
                                    movement_tableview.currentRow = styleData.row
                                    movement_tableview.focus = true

                                    switch(mouse.button)
                                    {
                                        case Qt.RightButton:
                                            console.log("[LOG] right click on item detected: ", tableView.currentRow)
                                            movement_contextMenu.popup() // Вызываем контексткное меню
                                            break
                                        default:
                                            break
                                    }
                                }
                            }
                        }
                    }

                    // Контекстно меню предлагает удаление строки из базы данных
                    Menu {
                        id: movement_contextMenu

                        MenuItem {
                            text: qsTr("Удалить")
                            onTriggered: {
                                /* Вызываем диалоговое окно,
                                 * которое уточнит намерение удалить строку из базы данных
                                 * */
                                movement_dialogDelete.open()
                            }
                        }
                    }

                    // Диалог подтверждения удаления строки из базы данных
                    MessageDialog {
                        id: movement_dialogDelete
                        title: qsTr("Удаление записи")
                        text: qsTr("Подтвердите удаление записи из журнала")
                        icon: StandardIcon.Warning
                        standardButtons: StandardButton.Ok | StandardButton.Cancel

                        // При положительном ответе ...
                        onAccepted: {
                            /* ... удаляем строку по id,
                             * который забираем из модели данных
                             * по номеру строки в представлении
                             * */
                            database.removeRecord("MovementList", movementModel.getId(movement_tableview.currentRow))
                            movementModel.updateModel(myModel.getId(tableView.currentRow));  // Обновляем модель данных
                        }
                    }

                    Button {
                        id:add_inv_button
                        text: qsTr("Добавить перемещение")
                        anchors.top: movement_tableview.bottom
                        anchors.topMargin: 3
                        anchors.right: movement_tableview.right
                        anchors.rightMargin: 3
                        visible:false

                        onClicked:
                        {
                            add_mov_win.show()
                        }
                    }
                }

                Rectangle {
                    id: invasions_column
                    anchors.top: parent.top
                    anchors.topMargin: 0                    
                    width: 450
                    height:250
                    color: "RED"

                    //данные
                    TableView {
                        id: invasions_tableView
                        width: 450
                        height:250

                        TableViewColumn {
                            id:invasion_name
                            role: "koronarogr_name"
                            title: "Воздействие"
                        }

                        TableViewColumn {
                            id:invasion_type
                            role: "inv_typename"
                            title: "Тип"
                        }

                        TableViewColumn {
                            id:invasion_datetime
                            role: "invasion_datetime"
                            title: "Дата Время"
                        }
                        model:invasionModel

                        // Настройка строки в TableView для перехавата левого клика мыши
                        rowDelegate: Rectangle {
                            anchors.fill: parent
                            color: styleData.selected ? 'skyblue' : (styleData.alternate ? 'whitesmoke' : 'white');
                            MouseArea {
                                anchors.fill: parent
                                acceptedButtons: Qt.RightButton | Qt.LeftButton
                                onClicked: {
                                    invasions_tableView.selection.clear()
                                    invasions_tableView.selection.select(styleData.row)
                                    invasions_tableView.currentRow = styleData.row
                                    invasions_tableView.focus = true

                                    switch(mouse.button)
                                    {
                                        case Qt.RightButton:
                                            console.log("[LOG] right click on item detected: ", tableView.currentRow)
                                            invasions_contextMenu.popup() // Вызываем контексткное меню
                                            break
                                        default:
                                            break
                                    }
                                }
                            }
                        }
                    }

                    // Контекстно меню предлагает удаление строки из базы данных
                    Menu {
                        id: invasions_contextMenu

                        MenuItem {
                            text: qsTr("Удалить")
                            onTriggered: {
                                /* Вызываем диалоговое окно,
                                 * которое уточнит намерение удалить строку из базы данных
                                 * */
                                invasions_dialogDelete.open()
                            }
                        }
                    }

                    // Диалог подтверждения удаления строки из базы данных
                    MessageDialog {
                        id: invasions_dialogDelete
                        title: qsTr("Удаление записи")
                        text: qsTr("Подтвердите удаление записи из журнала")
                        icon: StandardIcon.Warning
                        standardButtons: StandardButton.Ok | StandardButton.Cancel

                        // При положительном ответе ...
                        onAccepted: {
                            /* ... удаляем строку по id,
                             * который забираем из модели данных
                             * по номеру строки в представлении
                             * */
                            database.removeRecord("InvasionsList", invasionModel.getId(invasions_tableView.currentRow))
                            invasionModel.updateModel(myModel.getId(tableView.currentRow));  // Обновляем модель данных
                        }
                    }

                    Button {
                        id:add_mov_button
                        text: qsTr("Добавить процедуру")
                        anchors.top: invasions_tableView.bottom
                        anchors.topMargin: 3
                        anchors.right: invasions_tableView.right
                        anchors.rightMargin: 3
                        visible:false

                        onClicked:
                        {                            
                            add_inv_win.show()
                        }

                    }
                }
            }
            // /данные о клиенте
        }



        ///третий ряд - таблица с данными
        Row {
            id: row3_tableView
            anchors.top: new_client_window.bottom
            anchors.topMargin: 5
            anchors.left: parent.left
            anchors.leftMargin: 2
            visible: false  //не видимый по-умолчанию элемент

            //данные
            TableView {
                id: tableView
                width: 1004
                height: 380

                TableViewColumn {
                    id:surname
                    role: "surname"
                    title: "Фамилия"
                }
                TableViewColumn {
                    id:name
                    role: "name"
                    title: "Имя"
                }
                TableViewColumn {
                    id:pname
                    role: "pname"
                    title: "Отчество"
                }
                TableViewColumn {
                    id:age
                    role: "age"
                    title: "Возраст"
                }
                TableViewColumn {
                    id:historynumber
                    role: "historynumber"
                    title: "История болезни"
                }
                TableViewColumn {
                    id:hospital_date
                    role: "hospital_date"
                    title: "Дата госпитализации"
                }
                TableViewColumn {
                    id:diagnoz_oks
                    role: "diagnoz_oks"
                    title: "ОКС"
                }
                TableViewColumn {
                    id:outcome
                    role: "outcome_name"
                    title: "Исход"
                }

                TableViewColumn {
                    id:outcome_date
                    role: "outcome_date"
                    title: "Дата и время исхода"
                }

                model: myModel

                /*!
                  * \brief Функция очищает поля с данными пациента
                  */
                function clear_chekedit_fields()
                {
                    snameField.text = qsTr("Фамилия")
                    fnameField.text = ""
                    patField.text = ""
                    ageField.text = ""
                    history_number.text = ""
                    box_model_outcome.currentIndex = 0
                    oks.checked = true
                    unprofile.checked = false
                }

                /*!
                  * \brief Функция заполняет поля с данными пацента данными из модели
                  * \param [in] row_to_fill - номер ряда в модели из которого будут взяты данные для заполнения
                  */
                function fill_textedit_from_data(row_to_fill)
                {
                    var data_from_this_row = myModel.get(row_to_fill);                    
                    invasionModel.updateModel(myModel.getId(row_to_fill))
                    movementModel.updateModel(myModel.getId(row_to_fill))

                    console.log("fill_textedit_from_data() for id = ", row_to_fill)
                    console.log(data_from_this_row.surname)
                    console.log(data_from_this_row.historynumber)

                    //очистил все поля, чтобы не оставались поля от предъидущего клиента
                    clear_chekedit_fields()

                    snameField.text = data_from_this_row.surname
                    fnameField.text = data_from_this_row.name
                    patField.text = data_from_this_row.pname

                    var hospital_date_time_str_array = data_from_this_row.hospital_date.split(" ")
                    if (hospital_date_time_str_array.length >= 1)
                        date_hospital.text = hospital_date_time_str_array[0]
                    if (hospital_date_time_str_array.lengt === 2)
                        time_hospital.text = hospital_date_time_str_array[1]

                    ageField.text = data_from_this_row.age
                    history_number.text = data_from_this_row.historynumber

                    if (data_from_this_row.diagnoz_oks === "true")
                    {
                        oks.checked = true
                        unprofile.checked = false
                    }
                    if (data_from_this_row.diagnoz_oks === "false")
                    {
                        unprofile.checked = true
                        oks.checked = false
                    }

                    if (data_from_this_row.outcome !== 0)
                    {
                        box_model_outcome.currentIndex = data_from_this_row.outcome

                        var outcome_date_time_str_array = data_from_this_row.outcome_date.split(" ")
                        if (outcome_date_time_str_array.length >= 1)
                            date_outcome.text = outcome_date_time_str_array[0]
                        if (outcome_date_time_str_array.length === 2)
                            time_outcome.text = outcome_date_time_str_array[1]
                    }


                }

                // Настройка строки в TableView для перехавата левого клика мыши
                rowDelegate: Rectangle {
                    anchors.fill: parent
                    color: styleData.selected ? 'skyblue' : (styleData.alternate ? 'whitesmoke' : 'white');
                    MouseArea {
                        anchors.fill: parent
                        acceptedButtons: Qt.RightButton | Qt.LeftButton
                        onClicked: {
                            tableView.selection.clear()
                            tableView.selection.select(styleData.row)
                            tableView.currentRow = styleData.row
                            tableView.focus = true
                            //отобразить кнопки добваления информации о перемещении и процедурах
                            add_inv_button.visible = true
                            add_mov_button.visible = true

                            switch(mouse.button)
                            {
                                case Qt.RightButton:
                                    console.log("[LOG] right click on item detected: ", tableView.currentRow)
                                    contextMenu.popup() // Вызываем контексткное меню
                                    break
                                case Qt.LeftButton:
                                    console.log("[LOG] left click on item detected: ", tableView.currentRow)
                                    tableView.fill_textedit_from_data(tableView.currentRow)


                                    break
                                default:
                                    break
                            }
                        }
                    }
                }

                function update_table_width()
                {
                    surname.resizeToContents()
                    name.resizeToContents()
                    pname.resizeToContents()
                    age.resizeToContents()
                    historynumber.resizeToContents()
                    hospital_date.resizeToContents()
                    diagnoz_oks.resizeToContents()
                    outcome.resizeToContents()
                }

                function actual_update()
                {
                    myModel.actual_patients_updateModel();  // Обновляем модель данных
                    tableView.update_table_width();
                }

                function update()
                {
                    myModel.updateModel();  // Обновляем модель данных
                    tableView.update_table_width();
                }
            }

            // Контекстно меню предлагает удаление строки из базы данных
            Menu {
                id: contextMenu

                MenuItem {
                    text: qsTr("Удалить")
                    onTriggered: {
                        /* Вызываем диалоговое окно,
                         * которое уточнит намерение удалить строку из базы данных
                         * */
                        dialogDelete.open()
                    }
                }
                MenuItem {
                    text: qsTr("Редактировать")
                    onTriggered:
                    {
                        add_client_button.current_regime = 2
                    }
                }
            }

            // Диалог подтверждения удаления строки из базы данных
            MessageDialog {
                id: dialogDelete
                title: qsTr("Удаление записи")
                text: qsTr("Подтвердите удаление записи из журнала")
                icon: StandardIcon.Warning
                standardButtons: StandardButton.Ok | StandardButton.Cancel

                // При положительном ответе ...
                onAccepted: {
                    /* ... удаляем строку по id,
                     * который забираем из модели данных
                     * по номеру строки в представлении
                     * */
                    database.removeRecord("PatientList", myModel.getId(tableView.currentRow))
                    myModel.updateModel();  // Обновляем модель данных
                    invasionModel.updateModel(myModel.getId(tableView.currentRow));
                    movementModel.updateModel(myModel.getId(tableView.currentRow));
                }
            }
         }
       }
    }

    /*!
       \brief Окно отображения диалога добавления процедуры
      */
    Window {
        id: add_inv_win
        title: "Добавление процедуры"
        width: 320
        height: 240
        maximumHeight: 240
        minimumHeight: 240
        maximumWidth:  320
        minimumWidth:  320

        function set_date(newdate)
        {
            invasion_date.text = newdate
        }

        Rectangle {
            id: main_field_invasion
            anchors.top: parent.top
            anchors.topMargin: 10
            anchors.left: parent.left
            anchors.leftMargin: 10

            function make_invasion(new_client)
            {
                /// Формирование структуры с данными для передачи в cpp
                new_client.id_pat = myModel.getId(tableView.currentRow)
                new_client.invasion_name = box_procedure_list.currentIndex
                new_client.invasion_type = invasion_type_box.currentIndex
                new_client.invasion_datetime = Qt.formatDate(global_vars.dateInvasion,  "yyyy-MM-dd") + " " + invasion_time.text.toString()
            }
            
            /// Проверка данных, введенных в форму добавления процедуры
            function check_new_invasion(new_client)
            {
                if ((box_procedure_list.currentIndex === 1) || (box_procedure_list.currentIndex === 2) || (box_procedure_list.currentIndex === 3))
                    if (invasion_type_box.currentIndex != 1)
                        if (invasion_type_box.currentIndex != 2)
                            if (invasion_type_box.currentIndex != 3)
                                return 1;

                if (box_procedure_list.currentIndex === 4)
                    if (invasion_type_box.currentIndex === 4)
                        if (invasion_type_box.currentIndex === 5)
                            if (invasion_type_box.currentIndex === 6)
                                return 2;

                if (box_procedure_list.currentIndex === 6)
                    if (invasion_type_box.currentIndex != 0)
                        return 3;

                if (box_procedure_list.currentIndex === 0)
                    return 4;
            
                return 0;            
            }

            GridLayout {
              id: main_grid

              Text {
                    text: qsTr("Процедура")
                    Layout.row: 1
                    Layout.column: 1
                }
                ComboBox {
                    id: box_procedure_list
                    anchors.topMargin: 5
                    width: 200
                    model: model_invasions
                    textRole: "display"
                    Layout.row: 1
                    Layout.column: 2
                }

                Text {
                    text: qsTr("Тип процедуры")
                    Layout.row: 2
                    Layout.column: 1

                }

                ComboBox {
                    id: invasion_type_box
                    anchors.topMargin: 5
                    width: 200
                    model: model_invtypes
                    textRole: "display"
                    Layout.row: 2
                    Layout.column: 2
                }

                Text {
                    text: qsTr("Дата процедуры")
                    Layout.row: 3
                    Layout.column: 1
                }

                Button {
                    id: invasion_date
                    text: Qt.formatDate(global_vars.dateInvasion, "dd.MM.yyyy");
                    Layout.row: 3
                    Layout.column: 2
                    onClicked: {
                        dialogCalendar.show(global_vars.dateInvasion)
                        global_vars.set_date_flag(2)
                    }
                }

                Text {
                    text: qsTr("Время процедуры")
                    Layout.row: 4
                    Layout.column: 1
                }
                TextField
                {
                    id: invasion_time
                    width:50
                    Layout.row: 4
                    Layout.column: 2
                    validator: RegExpValidator { regExp: /(^([0,1][0-9])|(2[0-3])):([0-5]{1}[0-9]{1})/}
                }
                
                MessageDialog {
                    id:add_invasion_error_dialog
                    title: "Ошибка"
                    text: "Ошибка при попытке добавления процедуры"
                    icon: StandardIcon.Critical
                    visible: false

                    function set_error_type (err_num)
                    {
                        switch (err_num)
                        {
                            case 1:
                            {
                                add_invasion_error_dialog.text = "Коронарография может быть только плановой, экстренной или отсроченной";
                                break;
                            }
                            case 2:
                            {
                                add_invasion_error_dialog.text = "Не выполненное вмешатлеьство может быть только по причине противопоказаний, отказа больного или отсутствия показаний";
                                break;
                            }
                            case 3:
                            {
                                add_invasion_error_dialog.text = "Для тромболизиса не надо заполнять тип процедуры";
                                break;
                            }
                            case 4:
                            {
                                add_invasion_error_dialog.text = "Нельзя добавлять пустую процедуру";
                                break;
                            }
                        }
                    }
                }

                Button {
                    id:add_new_invasion
                    text: "Добавить"
                    Layout.row: 5
                    Layout.column: 1

                    onClicked:
                    {
                        var new_invasion = {};
                        main_field_invasion.make_invasion(new_invasion);
                        
                        var result_of_check = main_field_invasion.check_new_invasion(new_invasion)
                        if (result_of_check !== 0)
                        {
                            add_invasion_error_dialog.set_error_type(result_of_check)
                            add_invasion_error_dialog.open()
                        }
                        else
                        {
                            database.inserIntoTableInvasions(new_invasion)
                            invasionModel.updateModel(myModel.getId(tableView.currentRow));
                        }
                        add_inv_win.close()
                    }
                }

                Button {
                    id:close_new_invasion
                    text: "Отмена"
                    Layout.row: 5
                    Layout.column: 2

                    onClicked:
                    {
                        add_inv_win.close()
                    }
                }

            }
        }
    }
    //


    /*!
       \brief Окно отображения диалога добавления перемещения
      */
    Window {
        id: add_mov_win
        title: "Добавление перемещения"
        width: 320
        height: 240
        maximumHeight: 240
        minimumHeight: 240
        maximumWidth:  320
        minimumWidth:  320

        function set_date(newdate)
        {
            movement_date.text = newdate
        }

        Rectangle {
            id: main_field_movement
            anchors.top: parent.top
            anchors.topMargin: 10
            anchors.left: parent.left
            anchors.leftMargin: 10

            function make_movement(new_mov)
            {
                /// Формирование структуры с данными для передачи в cpp
                new_mov.id_pat = myModel.getId(tableView.currentRow)
                new_mov.new_storage = box_movement_list.currentIndex
                new_mov.movement_datetime = Qt.formatDate(global_vars.dateMovement, "yyyy-MM-dd") + " " + movement_time.text.toString();
            }

            GridLayout {
              id: main_grid_movement

              Text {
                    text: qsTr("Переведен в")
                    Layout.row: 1
                    Layout.column: 1
                }
                ComboBox {
                    id: box_movement_list
                    anchors.topMargin: 5
                    width: 200
                    model: model_storages
                    textRole: "display"
                    Layout.row: 1
                    Layout.column: 2
                }

                Text {
                    text: qsTr("Дата перевода")
                    Layout.row: 2
                    Layout.column: 1
                }

                Button {
                    id: movement_date
                    text: Qt.formatDate(global_vars.dateMovement, "dd.MM.yyyy");
                    Layout.row: 2
                    Layout.column: 2
                    onClicked: {
                        dialogCalendar.show(global_vars.dateMovement)
                        global_vars.set_date_flag(3)
                    }
                }

                Text {
                    text: qsTr("Время перевода")
                    Layout.row: 3
                    Layout.column: 1
                }
                TextField
                {
                    id: movement_time
                    width:50
                    Layout.row: 3
                    Layout.column: 2
                    validator: RegExpValidator { regExp: /(^([0,1][0-9])|(2[0-3])):([0-5]{1}[0-9]{1})/}
                }

                Button {
                    id:add_new_movement
                    text: "Добавить"
                    Layout.row: 4
                    Layout.column: 1

                    onClicked:
                    {
                        var new_mov = {};
                        main_field_movement.make_movement(new_mov);
                        database.inserIntoTableMovements(new_mov);
                        movementModel.updateModel(myModel.getId(tableView.currentRow));
                        add_mov_win.close();
                    }
                }

                Button {
                    id:close_new_movement
                    text: "Отмена"
                    Layout.row: 4
                    Layout.column: 2

                    onClicked:
                    {
                        add_mov_win.close()
                    }
                }

            }
        }
    }
}
