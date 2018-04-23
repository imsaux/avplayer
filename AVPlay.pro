#-------------------------------------------------
#
# Project created by QtCreator 2017-10-09T12:53:27
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AVPlay
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        widget.cpp \
    plabel.cpp \
    capture_thread.cpp \
    buffer.cpp \
    global.cpp \
    controller.cpp \
    distortion_thread.cpp \
    distortion_pack.cpp \
    distortionwidget.cpp


HEADERS += \
        widget.h \
    plabel.h \
    capture_thread.h \
    buffer.h \
    global.h \
    controller.h \
    distortion_thread.h \
    C:\Users\sy\Downloads\avplayer-master\opencv2\opencv.hpp \
    distortion_pack.h \
    distortionwidget.h

FORMS += \
        widget.ui \
    distortionwidget.ui



DISTFILES += \
    video.ini


INCLUDEPATH += ./inc
INCLUDEPATH += ./LIBS

#LIBS += -LC:\Users\sy\Downloads\avplayer-master\opencv2 -lopencv_core2410d -lopencv_highgui2410d -lopencv_imgproc2410d

win32: LIBS += -L$$PWD/lib/ -llibEasyPlayer

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.

win32: LIBS += -L$$PWD/inc/ -lDistortion

INCLUDEPATH += $$PWD/inc
DEPENDPATH += $$PWD/inc
