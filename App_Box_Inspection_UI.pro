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

INCLUDEPATH += ./module/ImageBuf2Mat ./module/EnsembleAPI ./module/EnsembleAPI/src ./module/EnsembleAPI/src/interface ./module/EnsembleAPI/module/PacketEthComm ./module/EnsembleAPI/module/PacketEthComm/Client ./module/EnsembleAPI/module/EnsembleCommon ./module/EnsembleAPI/module/EnsembleCommand ./module/ImgEncDec ./module/pugixml/src
LIBS += -lboost_system -lboost_thread -lboost_filesystem -lboost_regex -lboost_date_time

CONFIG += link_pkgconfig
PKGCONFIG += opencv4

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    module/EnsembleAPI/src/interface/Interface.cpp \
    module/EnsembleAPI/src/interface/InterfaceControl.cpp \
    module/EnsembleAPI/src/EnsembleAPI.cpp \
    module/ImgEncDec/ImgDec.cpp \
    module/ImgEncDec/ImgEnc.cpp \
    module/EnsembleAPI/module/PacketEthComm/Client/EthernetClient.cpp \
    module/EnsembleAPI/module/PacketEthComm/Client/EthernetClientControlData.cpp \
    module/EnsembleAPI/module/PacketEthComm/EthernetGetInfo.cpp \
    module/EnsembleAPI/module/PacketEthComm/RendezvueCheckData.cpp \
    censemble.cpp \
    cmat2qimage.cpp \
    CIniInfo.cpp \
    module/pugixml/src/pugixml.cpp \
    csavelogfile.cpp \
    module/ImageBuf2Mat/CImageBuf2Mat.cpp \
    CheckLogSize.cpp

HEADERS += \
        mainwindow.h \
    module/EnsembleAPI/src/interface/Interface.h \
    module/EnsembleAPI/src/interface/InterfaceControl.h \
    module/EnsembleAPI/src/EnsembleAPI.h \
    module/EnsembleAPI/module/EnsembleCommand/EnsembleCommand.h \
    module/EnsembleAPI/module/EnsembleCommon/EnsembleCommon.h \
    module/ImgEncDec/ImgDec.h \
    module/ImgEncDec/ImgEnc.h \
    module/EnsembleAPI/module/PacketEthComm/Client/EthernetClient.h \
    module/EnsembleAPI/module/PacketEthComm/Client/EthernetClientControlData.h \
    module/EnsembleAPI/module/PacketEthComm/ErrorType.h \
    module/EnsembleAPI/module/PacketEthComm/EthernetGetInfo.h \
    module/EnsembleAPI/module/PacketEthComm/RendezvueCheckData.h \
    censemble.h \
    cmat2qimage.h \
    CIniInfo.h \
    module/pugixml/src/pugiconfig.hpp \
    module/pugixml/src/pugixml.hpp \
    csavelogfile.h \
    module/ImageBuf2Mat/CImageBuf2Mat.h \
    CheckLogSize.h

FORMS += \
        mainwindow.ui
