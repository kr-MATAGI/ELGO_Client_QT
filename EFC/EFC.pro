QT -= gui
QT += core network


CONFIG += c++11
CONFIG += staticlib

TEMPLATE = lib

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    Common/CommonDef.h \
    Common/Deifinition.h \
    Common/DBDefinition.h \
    Common/EventList.h \
    Common/Interface/ContentsPlayDataDef.h \
    Common/Interface/ContentsPlayDataImpl.h \
    Common/Interface/ScheduleDef.h \
    Common/Interface/ScheduleImpl.h \
    LocalSocketEvent/EFCEvent.h \
    LocalSocketEvent/EventState.h \
    LocalSocketEvent/LocalSocketServer.h \
    Logger/ELogger.h \
    ShardMem/ShmCtrl.h \
    ShardMem/ShmDefinition.h


SOURCES += \
    Common/Interface/ContentsPlayDataImpl.cpp \
    Common/Interface/ScheduleImpl.cpp \
    LocalSocketEvent/EFCEvent.cpp \
    LocalSocketEvent/LocalSocketServer.cpp \
    Logger/ELogger.cpp \
    ShardMem/ShmCtrl.cpp
