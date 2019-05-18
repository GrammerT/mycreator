depfile = $$replace(_PRO_FILE_PWD_, ([^/]+$), \\1/\\1_dependencies.pri)
exists($$depfile) {
    include($$depfile)
    isEmpty(UNIZ_PLUGIN_NAME): \
        error("$$basename(depfile) does not define UNIZ_PLUGIN_NAME.")
} else {
    isEmpty(UNIZ_PLUGIN_NAME): \
        error("UNIZ_PLUGIN_NAME is empty. Maybe you meant to create $$basename(depfile)?")
}
TARGET = $$UNIZ_PLUGIN_NAME


plugin_deps = $$UNIZ_PLUGIN_DEPENDS
message(plugin_deps:$$plugin_deps)

include(../dll.pri)


defineReplace(dependencyName) {
    dependencies_file =
    for(dir, UNIZ_PLUGIN_DIRS) {
        exists($$dir/$$1/$${1}_dependencies.pri) {
            dependencies_file = $$dir/$$1/$${1}_dependencies.pri
            break()
        }
    }
    isEmpty(dependencies_file): \
        error("Plugin dependency $$dep not found")
    include($$dependencies_file)
    return($$UNIZ_PLUGIN_NAME)
}

# for substitution in the .json
dependencyList =
for(dep, plugin_deps) {
    dependencyList += "        { \"Name\" : \"$$dependencyName($$dep)\", \"Version\" : \"$$UNIZ_VERSION\" }"
}
for(dep, plugin_recmds) {
    dependencyList += "        { \"Name\" : \"$$dependencyName($$dep)\", \"Version\" : \"$$UNIZ_VERSION\", \"Type\" : \"optional\" }"
}
for(dep, plugin_test_deps) {
    dependencyList += "        { \"Name\" : \"$$dependencyName($$dep)\", \"Version\" : \"$$UNIZ_VERSION\", \"Type\" : \"test\" }"
}
dependencyList = $$join(dependencyList, ",$$escape_expand(\\n)")

dependencyList = "\"Dependencies\" : [$$escape_expand(\\n)$$dependencyList$$escape_expand(\\n)    ]"



isEmpty(USE_USER_DESTDIR) {
    DESTDIR = $$UNIZ_PLUGIN_BIN
    message(---DESTDIR:$$DESTDIR) # D:/Opensource workspace/dllSystem/lib/plugins
} else {
    win32 {
        DESTDIRAPPNAME = "Uniz"
        DESTDIRBASE = "$$(LOCALAPPDATA)"
        isEmpty(DESTDIRBASE):DESTDIRBASE="$$(USERPROFILE)\Local Settings\Application Data"
    } else:macx {
        DESTDIRAPPNAME = "Uniz"
        DESTDIRBASE = "$$(HOME)/Library/Application Support"
    }
    DESTDIR = "$$DESTDIRBASE/Uniz/$$DESTDIRAPPNAME/plugins/$$UNIZ_VERSION"
    message(DESTDIR:---$$DESTDIR)
}
LIBS += -L$$DESTDIR
INCLUDEPATH += $$OUT_PWD
message(OUT_PWD:---$$OUT_PWD) # D:/Opensource workspace/dllSystem/src/core

# copy the plugin spec
isEmpty(TARGET) {
    error("dllplugin.pri: You must provide a TARGET")
}

PLUGINJSON = $$_PRO_FILE_PWD_/$${TARGET}.json
PLUGINJSON_IN = $${PLUGINJSON}.in
exists($$PLUGINJSON_IN) {
    DISTFILES += $$PLUGINJSON_IN
    QMAKE_SUBSTITUTES += $$PLUGINJSON_IN
    PLUGINJSON = $$OUT_PWD/$${TARGET}.json
} else {
    # need to support that for external plugins
    DISTFILES += $$PLUGINJSON
}

contains(QT_CONFIG, reduce_exports):CONFIG += hide_symbols

TEMPLATE = lib
CONFIG += plugin plugin_with_soname
!macx {
    target.path = $$INSTALL_PLUGIN_PATH
    INSTALLS += target
}

TARGET = $$qtLibraryName($$TARGET)

