#ifndef DB_CONSTS
#define DB_CONSTS

/* Директивы имен таблицы, полей таблицы и базы данных */
#define DATABASE_HOSTNAME       "kotobase"
#define DATABASE_NAME           "kotobase.db"

#define TABLE_PATIENTS          "PatientList"         // Название таблицы
#define TABLENAME_KORONAROGR        "Koronarogr"
#define TABLENAME_OUTCOME           "Outcome"

#define TABLE_ID                "id"
#define TABLE_OUTCOME_NAME      "outcome_name"
#define TABLE_KORONAROGR_NAME   "koronarogr_name"

#define TABLE_FNAME             "surname"         // Вторая колонка
#define TABLE_SNAME             "name"          // Третья колонка
#define TABLE_PNAME             "pname"               // Четвертая колонка
#define TABLE_AGE               "age"               // Четвертая колонка
#define TABLE_HISTORYNUMBER     "historynumber"
#define TABLE_HOSPITALDATE      "hospital_date"
#define TABLE_DIAGNOZ_OKS       "diagnoz_oks"
#define TABLE_OUTCOME           "outcome"
#define TABLE_OUTCOME_JOIN      "outcome_name"
#define TABLE_OUTCOMEDATE       "outcome_date"


#define TABLE_INVASIONS_NAME    "InvasionsList"
#define INV_PATIENT_ID           "id_pat"
#define INV_INVASION_NAME        "invasion_name"
#define INV_INVASION_TYPE        "invasion_type"
#define INV_INVASION_DATETIME    "invasion_datetime"


#define TABLE_MOVEMENTS_NAME    "MovementList"
#define MOV_ID_PAT              "id_pat"
#define MOV_NEW_STORAGE         "new_storage"
#define MOV_DATETIME            "movement_datetime"

#define TABLE_STORAGES_NAME     "Storages"
#define STO_STORAGES_NAME       "storage_name"

#define TABLE_INVASIONTYPES_NAME    "InvasionTypes"
#define TABLE_INVASIONTYPES_FNAME    "inv_typename"

#endif // DB_CONSTS

