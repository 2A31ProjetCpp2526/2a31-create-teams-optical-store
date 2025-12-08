QT += core gui sql charts printsupport serialport widgets network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
#core5compat
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

# OpenCV - UPDATED PATH
# OpenCV - UPDATED PATH (QUOTED)

INCLUDEPATH += $$PWD/opencv_mingw/include

LIBS += -L $$PWD/opencv_mingw/x64/mingw/lib \
        -lopencv_core455 \
        -lopencv_imgproc455 \
        -lopencv_highgui455 \
        -lopencv_videoio455 \
        -lopencv_imgcodecs455 \
        -lopencv_calib3d455 \
        -lopencv_features2d455 \
        -lopencv_flann455 \
        -lopencv_dnn455

OPENCV_DLL_DIR = $$PWD/opencv_mingw/x64/mingw/bin

QMAKE_POST_LINK += xcopy /s /y \"$${OPENCV_DLL_DIR}\\*.dll\" \"$$OUT_PWD\"



# Debug OpenCV paths
!equals(_OPENCV_DEBUG, 1) {
    _OPENCV_DEBUG = 1
    message("=== OPENCV DEBUG ===")
    OPENCV_INC = $$PWD/opencv_mingw/include
    OPENCV_LIB = $$PWD/opencv_mingw/x64/mingw/lib


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
