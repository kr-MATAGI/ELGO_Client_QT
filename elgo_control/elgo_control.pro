QT -= gui

QT += network core

CONFIG += c++11 console
CONFIG -= app_bundle

LIBS += -LD:/OpenSSL-Win32/lib -openssl

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp \
        JSON/jsonparser.cpp \
        TCPConnect/networkCtrl.cpp

INCLUDEPATH += D:/OpenSSL-Win32/include

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    JSON/jsonparser.h \
    TCPConnect/Definition/serverInfo.h \
    TCPConnect/networkCtrl.h


#PKGCONFIG += openssl



