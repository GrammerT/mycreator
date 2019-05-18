DEFINES += EXTENSIONSYSTEM_LIBRARY
include(../dllpluginlibrary.pri)


HEADERS += \
    plugin.h \
    plugin_p.h \
    pluginmanager.h \
    pluginmanager_p.h \
    pluginspec.h \
    pluginspec_p.h

SOURCES += \
    plugin.cpp \
    pluginmanager.cpp \
    pluginspec.cpp
