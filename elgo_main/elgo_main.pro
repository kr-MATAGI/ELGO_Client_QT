QT -= gui

QT += network sql

CONFIG += c++11 console
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

HEADERS += \
    DB/DBDefinition.h \
    DB/MainDBCtrl.h \
    Definition/Definition.h \
    MainCtrl/MainCtrl.h

SOURCES += \
        DB/MainDBCtrl.cpp \
        Definition/DeviceDef.cpp \
        MainCtrl/MainCtrl.cpp \
        main.cpp

INCLUDEPATH += "C:/Project/Qt/ELGO_Client/EFC"
LIBS += "C:/Project/Qt/build-ELGO_Client-Desktop_Qt_5_15_2_MinGW_32_bit-Release/EFC/release/libEFC.a"

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
