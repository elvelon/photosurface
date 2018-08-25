TEMPLATE = app

QT += core qml quick network
CONFIG += c++11 testlib

SOURCES += main.cpp \
#    testimagecontext.cpp \
#    testdownloader.cpp \
    testping.cpp \
#    ../../imagecontext.cpp \
#    ../../downloader.cpp \
    ../../ping.cpp \


RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
#    testimagecontext.h \
#    testdownloader.h \
    testping.h \
#    ../../imagecontext.h \
#    ../../downloader.h \
    ../../ping.h \


