QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
QT += sql

SOURCES += \
    client.cpp \
    connection.cpp \
    main.cpp \
    opticstor.cpp \
    produit.cpp

HEADERS += \
    client.h \
    connection.h \
    opticstor.h \
    produit.h

FORMS += \
    opticstor.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
