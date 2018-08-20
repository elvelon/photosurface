TEMPLATE = app

QT += qml quick network
qtHaveModule(widgets): QT += widgets
SOURCES += main.cpp \
    downloader.cpp \
    functionality.cpp \
    ping.cpp \
    imagecontext.cpp \
    argumentparser.cpp \
    filelogger.cpp
RESOURCES += photosurface.qrc

TARGET = photosurface
ICON = resources/icon.png
macx: ICON = resources/photosurface.icns
win32: RC_FILE = resources/photosurface.rc

HEADERS += \
    downloader.h \
    functionality.h \
    ping.h \
    imagecontext.h \
    argumentparser.h \
    filelogger.h

DISTFILES += \
    android/AndroidManifest.xml \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradlew \
    android/res/values/libs.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew.bat

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

