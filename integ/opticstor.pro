QT       += core gui sql charts printsupport widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# Default: disable deprecated APIs
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000

SOURCES += \
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

# =========================================================
#                OPENCV UPDATED PATHS
# =========================================================

# NEW OpenCV base directory
OPENCV_BASE = "C:/Users/debbe/Downloads/integ (7)/integ/opencv_mingw"

# Includes
INCLUDEPATH += $$OPENCV_BASE/include

# Libraries
LIBS += -L $$OPENCV_BASE/x64/mingw/lib \
        -lopencv_core455 \
        -lopencv_imgproc455 \
        -lopencv_highgui455 \
        -lopencv_videoio455 \
        -lopencv_imgcodecs455 \
        -lopencv_calib3d455 \
        -lopencv_features2d455 \
        -lopencv_flann455 \
        -lopencv_dnn455

# DLLs for runtime
OPENCV_DLL_DIR = $$OPENCV_BASE/x64/mingw/bin
QMAKE_POST_LINK += xcopy /s /y \"$${OPENCV_DLL_DIR}\\*.dll\" \"$$OUT_PWD\"

# Debug info
!equals(_OPENCV_DEBUG, 1) {
    _OPENCV_DEBUG = 1
    message("=== OPENCV DEBUG ===")
    OPENCV_INC = $$OPENCV_BASE/include
    OPENCV_LIB = $$OPENCV_BASE/x64/mingw/lib
    message("OpenCV include: $$OPENCV_INC")
    message("OpenCV lib: $$OPENCV_LIB")

    LIB_EXISTS = $$system(dir "$$OPENCV_LIB/libopencv_core455.a" 2>nul)
    !isEmpty(LIB_EXISTS) {
        message("libopencv_core455.a: FOUND")
    } else {
        message("libopencv_core455.a: NOT FOUND")
    }
}

# Deployment
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
