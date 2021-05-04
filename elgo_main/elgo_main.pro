QT -= gui

QT += network sql xml

CONFIG += c++11 console
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

HEADERS += \
    DB/MainDBCtrl.h \
    Definition/Definition.h \
    Definition/WifiDef.h \
    Event/MainEventHandler.h \
    Event/MainEventState.h \
    Logger/MainLogger.h \
    MainCtrl/MainController.h \
    MainCtrl/MainCtrl.h \
    MainThread/MainThread.h \
    Timer/PlayScheduleTimer.h \
    Timer/PowerScheduleTimer.h \
    Utils/WifiManager.h \
    XML/XMLParser.h

SOURCES += \
        DB/MainDBCtrl.cpp \
        Definition/DeviceDef.cpp \
        Event/MainEventHandler.cpp \
        Event/MainEventState.cpp \
        MainCtrl/MainController.cpp \
        MainCtrl/MainCtrl.cpp \
        MainThread/MainThread.cpp \
        Timer/PlayScheduleTimer.cpp \
        Timer/PowerScheduleTimer.cpp \
        Utils/WifiManager.cpp \
        XML/XMLParser.cpp \
        main.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

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
