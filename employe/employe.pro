#-------------------------------------------------
# Project configuration
#-------------------------------------------------
QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
CONFIG += qt warn_on

TEMPLATE = app
TARGET = employe

#-------------------------------------------------
# Source files
#-------------------------------------------------
SOURCES += \
    main.cpp \
    mainwindow.cpp \
    employe.cpp \
    connexion.cpp

#-------------------------------------------------
# Header files
#-------------------------------------------------
HEADERS += \
    mainwindow.h \
    employe.h \
    connexion.h

#-------------------------------------------------
# UI files
#-------------------------------------------------
FORMS += \
    mainwindow.ui

#-------------------------------------------------
# Resources (optional, if you use .qrc for images/icons)
#-------------------------------------------------
# RESOURCES += resources.qrc

#-------------------------------------------------
# Deployment rules
#-------------------------------------------------
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

#-------------------------------------------------
# Additional settings
#-------------------------------------------------

# Ensures the application uses the correct character set
QMAKE_CXXFLAGS += -Wall

# If you want to include your SQLite database file in the build folder
# Copy your database file to the output directory after build
# (uncomment if needed)
# QMAKE_POST_LINK += $$quote(cp $$PWD/database.db $$OUT_PWD/)

# If you're using MySQL, ensure the driver plugin is available on your system.
# No extra driver config is needed for SQLite (QSQLITE is built in).
