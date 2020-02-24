#-------------------------------------------------
#
# Project created by QtCreator 2020-02-20T14:13:27
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = App_Box_Inspection_UI
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += ./module/EnsembleAPI ./module/EnsembleAPI/src ./module/PacketEthComm ./module/PacketEthComm/Client ./module/EnsembleCommon ./module/EnsembleCommand ./module/ImgEncDec
LIBS += -lboost_system -lboost_thread -lboost_filesystem -lboost_regex

CONFIG += link_pkgconfig
PKGCONFIG += opencv4

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    module/EnsembleAPI/src/Interface.cpp \
    module/EnsembleAPI/src/InterfaceControl.cpp \
    module/EnsembleAPI/EnsembleAPI.cpp \
    module/ImgEncDec/ImgDec.cpp \
    module/ImgEncDec/ImgEnc.cpp \
    module/PacketEthComm/Client/EthernetClient.cpp \
    module/PacketEthComm/Client/EthernetClientControlData.cpp \
    module/PacketEthComm/EthernetGetInfo.cpp \
    module/PacketEthComm/RendezvueCheckData.cpp \
    censemble.cpp \
    cmat2qimage.cpp

HEADERS += \
        mainwindow.h \
    module/EnsembleAPI/src/Interface.h \
    module/EnsembleAPI/src/InterfaceControl.h \
    module/EnsembleAPI/EnsembleAPI.h \
    module/EnsembleCommand/EnsembleCommand.h \
    module/EnsembleCommon/EnsembleCommon.h \
    module/ImgEncDec/ImgDec.h \
    module/ImgEncDec/ImgEnc.h \
    module/PacketEthComm/Client/EthernetClient.h \
    module/PacketEthComm/Client/EthernetClientControlData.h \
    module/PacketEthComm/ErrorType.h \
    module/PacketEthComm/EthernetGetInfo.h \
    module/PacketEthComm/RendezvueCheckData.h \
    censemble.h \
    cmat2qimage.h

FORMS += \
        mainwindow.ui

DISTFILES += \
    module/EnsembleAPI/README.md \
    module/ImgEncDec/README.md \
    module/EnsembleAPI/LICENSE \
    module/EnsembleCommand/LICENSE \
    module/EnsembleCommon/LICENSE \
    module/ImgEncDec/LICENSE \
    module/PacketEthComm/LICENSE
