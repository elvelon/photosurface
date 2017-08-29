TEMPLATE = app

QT += qml quick network
qtHaveModule(widgets): QT += widgets
SOURCES += main.cpp \
    downloader.cpp \
    functionality.cpp \
    ping.cpp
RESOURCES += photosurface.qrc

target.path = $$[QT_INSTALL_EXAMPLES]/quick/demos/photosurface
INSTALLS += target
ICON = resources/icon.png
macx: ICON = resources/photosurface.icns
win32: RC_FILE = resources/photosurface.rc

HEADERS += \
    downloader.h \
    functionality.h \
    ping.h

DISTFILES += \
    android/AndroidManifest.xml \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradlew \
    android/res/values/libs.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew.bat

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

