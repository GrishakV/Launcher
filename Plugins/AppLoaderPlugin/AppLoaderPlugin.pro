QT += core gui widgets network sql

TARGET = AppLoaderPlugin

TEMPLATE = lib
CONFIG += plugin

CONFIG += c++11


DEFINES += QT_DEPRECATED_WARNINGS

INCLUDEPATH  += ../../MainProjectWindow

CONFIG(debug,debug|release){
    BUILDTYPE = debug
}
else {
    BUILDTYPE = release
}

DESTDIR = ../../MainProjectWindow/$${BUILDTYPE}/plugins

SOURCES += \
    apploaderplugin.cpp \
    paginationproxymodel.cpp \
    paginationwidget.cpp \
    ping.cpp \
    pingwidget.cpp

HEADERS += \
    apploaderplugin.h \
    paginationproxymodel.h \
    paginationwidget.h \
    ping.h \
    pingwidget.h

DISTFILES += AppLoaderPlugin.json

# Default rules for deployment.
unix {
    target.path = $$[QT_INSTALL_PLUGINS]/generic
}
!isEmpty(target.path): INSTALLS += target
