!isEmpty(dllPlugin_PRI_INCLUDED):error("dllPlugin.pri already included")
dllPlugin_PRI_INCLUDED = 1
# dllPlugin system version
DLLPLUGIN_VERSION = 1.0.0
# dllPlugin 兼容的版本号
DLLPLUGIN_COMPAT_VERSION = 1.0.0


CONFIG += c++11

defineReplace(dllLibraryName) {
   unset(LIBRARY_NAME)
   LIBRARY_NAME = $$1
   CONFIG(debug, debug|release) {
      !debug_and_release|build_pass {
          mac:RET = $$member(LIBRARY_NAME, 0)_debug
              else:win32:RET = $$member(LIBRARY_NAME, 0)d
      }
   }
   isEmpty(RET):RET = $$LIBRARY_NAME
   return($$RET)
}


defineTest(minQtVersion) {
    maj = $$1
    min = $$2
    patch = $$3
    isEqual(QT_MAJOR_VERSION, $$maj) {
        isEqual(QT_MINOR_VERSION, $$min) {
            isEqual(QT_PATCH_VERSION, $$patch) {
                return(true)
            }
            greaterThan(QT_PATCH_VERSION, $$patch) {
                return(true)
            }
        }
        greaterThan(QT_MINOR_VERSION, $$min) {
            return(true)
        }
    }
    greaterThan(QT_MAJOR_VERSION, $$maj) {
        return(true)
    }
    return(false)
}

defineReplace(stripSrcDir){
    return($$relative_path($$absolute_path($$1, $$OUT_PWD), $$_PRO_FILE_PWD_))
}

isEmpty(IDE_LIBRARY_BASENAME) {
    IDE_LIBRARY_BASENAME = lib
}

IDE_SOURCE_TREE = $$PWD
isEmpty(IDE_BUILD_TREE) {
    sub_dir = $$_PRO_FILE_PWD_
    sub_dir ~= s,^$$re_escape($$PWD),,
    IDE_BUILD_TREE = $$clean_path($$OUT_PWD)
    IDE_BUILD_TREE ~= s,$$re_escape($$sub_dir)$,,
}


IDE_APP_PATH = $$IDE_BUILD_TREE/bin
osx{
}else{
    contains(TEMPLATE, vc.*):vcproj = 1
    IDE_APP_TARGET   = dllplugin

    # target output path if not set manually
    isEmpty(IDE_OUTPUT_PATH): IDE_OUTPUT_PATH = $$IDE_BUILD_TREE

    IDE_LIBRARY_PATH = $$IDE_OUTPUT_PATH/$$IDE_LIBRARY_BASENAME/dllplugin
    IDE_PLUGIN_PATH  = $$IDE_LIBRARY_PATH/plugins
    IDE_BIN_PATH     = $$IDE_OUTPUT_PATH/bin
    win32: \
        IDE_LIBEXEC_PATH = $$IDE_OUTPUT_PATH/bin
    else: \
        IDE_LIBEXEC_PATH = $$IDE_OUTPUT_PATH/libexec/dllplugin
    !isEqual(IDE_SOURCE_TREE, $$IDE_OUTPUT_PATH):copydata = 1

    LINK_LIBRARY_PATH = $$IDE_BUILD_TREE/$$IDE_LIBRARY_BASENAME/dllplugin
    LINK_PLUGIN_PATH  = $$LINK_LIBRARY_PATH/plugins

    INSTALL_LIBRARY_PATH = $$DLLP_PREFIX/$$IDE_LIBRARY_BASENAME/dllplugin
    INSTALL_PLUGIN_PATH  = $$INSTALL_LIBRARY_PATH/plugins

    win32: \
        INSTALL_LIBEXEC_PATH = $$DLLP_PREFIX/bin
    else: \
        INSTALL_LIBEXEC_PATH = $$DLLP_PREFIX/libexec/dllplugin

    INSTALL_BIN_PATH     = $$DLLP_PREFIX/bin

}

DLLP_PLUGIN_DIRS_FROM_ENVIRONMENT = $$(DLLP_PLUGIN_DIRS)
DLLP_PLUGIN_DIRS += $$split(DLLP_PLUGIN_DIRS_FROM_ENVIRONMENT, $$QMAKE_DIRLIST_SEP)
DLLP_PLUGIN_DIRS += $$IDE_SOURCE_TREE/src/plugins
for(dir, DLLP_PLUGIN_DIRS) {
    INCLUDEPATH += $$dir
}


CONFIG += \
    depend_includepath \
    no_include_pwd


LIBS *= -L$$LINK_LIBRARY_PATH
exists($$IDE_LIBRARY_PATH): LIBS *= -L$$IDE_LIBRARY_PATH  # library path from output path

!isEmpty(vcproj) {
    DEFINES += IDE_LIBRARY_BASENAME=\"$$IDE_LIBRARY_BASENAME\"
} else {
    DEFINES += IDE_LIBRARY_BASENAME=\\\"$$IDE_LIBRARY_BASENAME\\\"
}


msvc {
    #Don't warn about sprintf, fopen etc being 'unsafe'
    DEFINES += _CRT_SECURE_NO_WARNINGS
    QMAKE_CXXFLAGS_WARN_ON *= -w44996
    # Speed up startup time when debugging with cdb
    QMAKE_LFLAGS_DEBUG += /INCREMENTAL:NO
}


qt {
    contains(QT, core): QT += concurrent
    contains(QT, gui): QT += widgets
}

# recursively resolve plugin deps
# ever ：是个常量，永远为true
done_plugins =
for(ever) {
    isEmpty(DLLP_PLUGIN_DEPENDS): \
        break()
    done_plugins += $$DLLP_PLUGIN_DEPENDS
    for(dep, DLLP_PLUGIN_DEPENDS) {
        dependencies_file =
        for(dir, DLLP_PLUGIN_DIRS) {
            exists($$dir/$$dep/$${dep}_dependencies.pri) {
                dependencies_file = $$dir/$$dep/$${dep}_dependencies.pri
                break()
            }
        }
        isEmpty(dependencies_file): \
            error("Plugin dependency $$dep not found")
        include($$dependencies_file)
        LIBS += -l$$qtLibraryName($$DLLP_PLUGIN_NAME) # 调用qtLibraryName
    }
    DLLP_PLUGIN_DEPENDS = $$unique(DLLP_PLUGIN_DEPENDS)
    DLLP_PLUGIN_DEPENDS -= $$unique(done_plugins)
}

# recursively resolve library deps
done_libs =
for(ever) {
    isEmpty(DLLP_LIB_DEPENDS): \
        break()
    done_libs += $$DLLP_LIB_DEPENDS
    for(dep, DLLP_LIB_DEPENDS) {
        include($$PWD/src/libs/$$dep/$${dep}_dependencies.pri)
        LIBS += -l$$qtLibraryName($$DLLP_LIB_NAME)
    }
    DLLP_LIB_DEPENDS = $$unique(DLLP_LIB_DEPENDS)
    DLLP_LIB_DEPENDS -= $$unique(done_libs)
}




























