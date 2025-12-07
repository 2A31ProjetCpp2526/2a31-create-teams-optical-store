QT       += core gui sql charts printsupport widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
QT += serialport

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    arduino.cpp \
    camerawidget.cpp \
    cameraworker.cpp \
    client.cpp \
    commande.cpp \
    connection.cpp \
    employe.cpp \
    feedbackdialog.cpp \
    fournisseur.cpp \
    main.cpp \
    metier.cpp \
    opticstor.cpp \
    produit.cpp \
    ravitaillementdialog.cpp

HEADERS += \
    arduino.h \
    camerawidget.h \
    cameraworker.h \
    client.h \
    commande.h \
    connection.h \
    employe.h \
    feedbackdialog.h \
    fournisseur.h \
    metier.h \
    opticstor.h \
    produit.h \
    ravitaillementdialog.h

FORMS += \
    opticstor.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# OpenCV - UPDATED PATH
# INCLUDE PATH
INCLUDEPATH += C:/Users/Douaa/Desktop/bara/integ/integ/opencv_mingw/include

# LIBRARIES
LIBS += -L C:/Users/Douaa/Desktop/bara/integ/integ/opencv_mingw/x64/mingw/lib \
        -lopencv_core455 \
        -lopencv_imgproc455 \
        -lopencv_highgui455 \
        -lopencv_videoio455 \
        -lopencv_imgcodecs455 \
        -lopencv_calib3d455 \
        -lopencv_features2d455 \
        -lopencv_flann455 \
        -lopencv_dnn455

# DLL DIRECTORY (bin instead of lib)
OPENCV_DLL_DIR = C:/Users/Douaa/Desktop/bara/integ/integ/opencv_mingw/x64/mingw/bin

QMAKE_POST_LINK += xcopy /s /y \"$${OPENCV_DLL_DIR}\\*.dll\" \"$$OUT_PWD\"


# Debug OpenCV paths
!equals(_OPENCV_DEBUG, 1) {
    _OPENCV_DEBUG = 1
    message("=== OPENCV DEBUG ===")

    OPENCV_INC = C:/Users/Douaa/Desktop/bara/integ/integ/opencv_mingw/include
    OPENCV_LIB = C:/Users/Douaa/Desktop/bara/integ/integ/opencv_mingw/x64/mingw/lib

    message("OpenCV include: $$OPENCV_INC")
    message("OpenCV lib: $$OPENCV_LIB")

    # Check if files exist
    LIB_EXISTS = $$system(dir "$$OPENCV_LIB/libopencv_core455.a" 2>nul)
    !isEmpty(LIB_EXISTS) {
        message("libopencv_core455.a: FOUND")
    } else {
        message("libopencv_core455.a: NOT FOUND")
    }
}
