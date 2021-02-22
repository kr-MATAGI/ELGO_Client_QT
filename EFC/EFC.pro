QT -= gui
QT += core network


CONFIG += c++11 console
CONFIG += staticlib
CONFIG -= app_bundle

TARGET = EFC
TEMPLATE = lib


# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    IPC/Client/LocalClient.h \
    IPC/Server/LocalServer.h \
    Logger/ELogger.h \
    ShardMem/ShardMemory.h


SOURCES += \
    IPC/Client/LocalClient.cpp \
    IPC/Server/LocalServer.cpp \
    Logger/ELogger.cpp \
    ShardMem/ShardMemory.cpp \
    main.cpp
