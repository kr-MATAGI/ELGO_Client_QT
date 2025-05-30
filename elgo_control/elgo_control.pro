QT -= gui

QT += network core websockets sql

CONFIG += c++11 console
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        ControlThread/ControlThread.cpp \
        DB/NetworkDBCtrl.cpp \
        DownloadThread/CurlDownload.cpp \
        DownloadThread/DownloadThread.cpp \
        Timer/UpdateWidgetTimer.cpp \
        Utils/VideoInfoHelper.cpp \
        Event/ContorlEventHandler.cpp \
        Event/ControlEventState.cpp \
        JSON/JsonParser.cpp \
        JSON/JsonStringConverter.cpp \
        JSON/JsonWriter.cpp \
        NetworkCtrl/ContentWebSocket/ContentWebSocket.cpp \
        NetworkCtrl/ContentWebSocket/ContentWebSocketHandler.cpp \
        NetworkCtrl/NetworkController.cpp \
        NetworkCtrl/NetworkCtrl.cpp \
        NetworkCtrl/RemoteControl/RemoteControlHandler.cpp \
        NetworkCtrl/RemoteControl/RemoteControlServer.cpp \
        XML/XmlParser.cpp \
        main.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    ControlThread/ControlThread.h \
    DB/NetworkDBCtrl.h \
    Definition/WifiDef.h \
    DownloadThread/CurlDownload.h \
    DownloadThread/DownloadDef.h \
    DownloadThread/DownloadThread.h \
    Timer/UpdateWidgetTimer.h \
    Utils/VideoInfoHelper.h \
    Event/ControlEventHandler.h \
    Event/ControlEventState.h \
    JSON/JsonParser.h \
    JSON/JsonStringConverter.h \
    JSON/JsonWriter.h \
    JSON/Schema/ContentServerSchema.h \
    JSON/Schema/RemoteSchema.h \
    Logger/ControlLogger.h \
    NetworkCtrl/ContentWebSocket/ContentWebSocket.h \
    NetworkCtrl/ContentWebSocket/ContentWebSocketHandler.cpp \
    NetworkCtrl/ContentWebSocket/ContentWebSocketHandler.h \
    NetworkCtrl/NetworkController.h \
    NetworkCtrl/Definition/ServerInfo.h \
    NetworkCtrl/NetworkCtrl.h \
    NetworkCtrl/RemoteControl/RemoteControlHandler.h \
    NetworkCtrl/RemoteControl/RemoteControlServer.h \
    XML/XmlParser.h


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
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/ext_libs/libcurl/lib/release/ -lcurl
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/ext_libs/libcurl/lib/debug/ -lcurl
else:unix:!macx: LIBS += -L$$PWD/ext_libs/libcurl/lib/ -lcurl

INCLUDEPATH += $$PWD/ext_libs/libcurl/include
DEPENDPATH += $$PWD/ext_libs/libcurl/include

#ffmpeg - avcodec
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/ext_libs/ffmpeg/lib/release/ -lavcodec
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/ext_libs/ffmpeg/lib/debug/ -lavcodec
else:unix:!macx: LIBS += -L$$PWD/ext_libs/ffmpeg/lib/ -lavcodec

INCLUDEPATH += $$PWD/ext_libs/ffmpeg/include
DEPENDPATH += $$PWD/ext_libs/ffmpeg/include

#ffmpeg - avformat
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/ext_libs/ffmpeg/lib/release/ -lavformat
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/ext_libs/ffmpeg/lib/debug/ -lavformat
else:unix:!macx: LIBS += -L$$PWD/ext_libs/ffmpeg/lib/ -lavformat

INCLUDEPATH += $$PWD/ext_libs/ffmpeg/include
DEPENDPATH += $$PWD/ext_libs/ffmpeg/include

#ffmpeg - avutil
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/ext_libs/ffmpeg/lib/release/ -lavutil
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/ext_libs/ffmpeg/lib/debug/ -lavutil
else:unix:!macx: LIBS += -L$$PWD/ext_libs/ffmpeg/lib/ -lavutil

INCLUDEPATH += $$PWD/ext_libs/ffmpeg/include
DEPENDPATH += $$PWD/ext_libs/ffmpeg/include
