QT += quick multimedia svg

CONFIG += c++11

HEADERS += \
        Definition/ViewerDefinition.h \
        Event/ViewerEventHandler.h \
        Event/ViewerEventState.h \
        Logger/ViewerLogger.h \
        MainWindow.h \
        ViewerCtrl/ViewerController.h \
        ViewerCtrl/ViewerCtrl.h \
        ViewerThread/ViewerThread.h \
        Widget/Clock/ClockWidget.h \
        Widget/ContentsPlayer.h \
        Widget/Date/DateWidget.h \
        Widget/Definition/ContentsDef.h \
        Widget/News/NewsFeedWidget.h \
        Widget/Weather/WeatherWidget.h \
        ext_libs/QrCode/QrCode.h \
        QrCode/QrMaker.h


SOURCES += \
        Event/ViewerEventHandler.cpp \
        Event/ViewerEventState.cpp \
        MainWindow.cpp \
        ViewerCtrl/ViewerController.cpp \
        ViewerCtrl/ViewerCtrl.cpp \
        ViewerThread/ViewerThread.cpp \
        Widget/Clock/ClockWidget.cpp \
        Widget/ContentsPlayer.cpp \
        Widget/Date/DateWidget.cpp \
        Widget/News/NewsFeedWidget.cpp \
        Widget/Weather/WeatherWidget.cpp \
        main.cpp \
        ext_libs/QrCode/QrCode.cpp \
        QrCode/QrMaker.cpp

FORMS += \
    MainWindow.ui \
    Widget/Clock/ClockWidget.ui \
    Widget/ContentsPlayer.ui \
    Widget/Date/DateWidget.ui \
    Widget/News/NewsFeedWidget.ui \
    Widget/Weather/WeatherWidget.ui

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# EFC
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

RESOURCES += \
    resource.qrc
