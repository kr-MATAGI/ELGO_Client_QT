QT -= gui

QT += network core websockets sql

CONFIG += c++11 console
CONFIG -= app_bundle

win32 {
    LIBS += -LD:/OpenSSL-Win32/lib -lopenssl
    INCLUDEPATH += D:/OpenSSL-Win32/include
}
unix {
    PKGCONFIG += openssl
}

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        ControlThread/ControlThread.cpp \
        DB/NetworkDBCtrl.cpp \
        Event/ContorlEventHandler.cpp \
        Event/ControlEventState.cpp \
        JSON/JsonParser.cpp \
        NetworkCtrl/NetworkController.cpp \
        NetworkCtrl/NetworkCtrl.cpp \
        NetworkCtrl/RemoteControl/RemoteControlActionHandler.cpp \
        NetworkCtrl/RemoteControl/RemoteControlServer.cpp \
        main.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    ControlThread/ControlThread.h \
    DB/NetworkDBCtrl.h \
    Event/ControlEventHandler.h \
    Event/ControlEventState.h \
    JSON/JsonParser.h \
    JSON/Schema/RemoteSchema.h \
    Logger/ControlLogger.h \
    NetworkCtrl/NetworkController.h \
    NetworkCtrl/Definition/ServerInfo.h \
    NetworkCtrl/NetworkCtrl.h \
    NetworkCtrl/RemoteControl/RemoteControlActionHandler.h \
    NetworkCtrl/RemoteControl/RemoteControlServer.h


#INCLUDEPATH += "C:/Project/Qt/ELGO_Client/EFC"
#LIBS += "C:/Project/Qt/build-ELGO_Client-Desktop_Qt_5_15_2_MinGW_32_bit-Release/EFC/release/libEFC.a"



#EFC
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../EFC/release/ -lEFC
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../EFC/debug/ -lEFC
else:unix:!macx: LIBS += -L$$OUT_PWD/../EFC/ -lEFC

INCLUDEPATH += $$PWD/../EFC
DEPENDPATH += $$PWD/../EFC

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../EFC/release/libEFC.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../EFC/debug/libEFC.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../EFC/release/EFC.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../EFC/debug/EFC.lib
else:unix:!macx: PRE_TARGETDEPS += $$OUT_PWD/../EFC/libEFC.a

#libcurl
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../EFC/Common/ext_libs/curl/lib/release/ -lcurl
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../EFC/Common/ext_libs/curl/lib/debug/ -lcurl
else:unix:!macx: LIBS += -L$$PWD/../EFC/Common/ext_libs/curl/lib/ -lcurl

INCLUDEPATH += $$PWD/../EFC/Common/ext_libs/curl/include
DEPENDPATH += $$PWD/../EFC/Common/ext_libs/curl/include

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../EFC/Common/ext_libs/curl/lib/release/libcurl.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../EFC/Common/ext_libs/curl/lib/debug/libcurl.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../EFC/Common/ext_libs/curl/lib/release/curl.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../EFC/Common/ext_libs/curl/lib/debug/curl.lib
else:unix:!macx: PRE_TARGETDEPS += $$PWD/../EFC/Common/ext_libs/curl/lib/libcurl.a
