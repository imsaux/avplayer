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
    plabel.cpp

HEADERS += \
        widget.h \
    plabel.h

FORMS += \
        widget.ui


INCLUDEPATH += ./inc
#LIBS += -LE:\05.Projects\06.QTProjects\AVPlay\lib -llibEasyPlayer





win32:CONFIG(release, debug|release): LIBS += -L$$PWD/lib/ -llibEasyPlayer
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/lib/ -llibEasyPlayer

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/lib/liblibEasyPlayer.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/lib/liblibEasyPlayerd.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/lib/libEasyPlayer.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/lib/libEasyPlayer.lib

DISTFILES += \
    video.ini

win32: LIBS += -L$$PWD/inc/ -lDistortion

INCLUDEPATH += $$PWD/inc
DEPENDPATH += $$PWD/inc

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/inc/Distortion.lib
else:win32-g++: PRE_TARGETDEPS += $$PWD/inc/libDistortion.a
