QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

INCLUDEPATH += ~/bin/libxls/includep
#LIBS += ~/bin/libxl-3.9.1.0/lib64/libxl.so
LIBS += ~/bin/libxls/.libs/libxlsreader.so
LIBS += ~/bin/libxls/.libs/libxlsreader.so.8
# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Channel.cpp \
    DBCFile.cpp \
    DataCalcManager.cpp \
    DataChannel.cpp \
    DataManager.cpp \
    HeaderView.cpp \
    MDBFile.cpp \
    SignalValueDelegate.cpp \
    SocketMoudle.cpp \
    Utils.cpp \
    main.cpp \
    mainwindow.cpp \
    myfilterproxymodel.cpp

HEADERS += \
    Channel.h \
    ConQueue.h \
    DBCFile.h \
    DataCalcManager.h \
    DataChannel.h \
    DataManager.h \
    HeaderView.h \
    MDBFile.h \
    MsgFile.h \
    SignalValueDelegate.h \
    SocketMoudle.h \
    Utils.h \
    mainwindow.h \
    myfilterproxymodel.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

SUBDIRS += \
    Emulator.pro

DISTFILES += \
    Emulator.pro.user \
    VehicleEmulator.pro.user
