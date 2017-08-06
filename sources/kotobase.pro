TEMPLATE = app

QT += qml quick
QT += qml quick widgets sql
CONFIG += c++11
QT += printsupport

SOURCES += main.cpp \
    database.cpp \
    listmodel.cpp \
    reporter.cpp \
    comboboxsql.cpp \
    paramstorage.cpp \
    invasionlist.cpp \
    movementlist.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    database.h \
    listmodel.h \
    db_consts.h \
    reporter.h \
    comboboxsql.h \
    paramstorage.h \
    invasionlist.h \
    movementlist.h

DISTFILES +=

