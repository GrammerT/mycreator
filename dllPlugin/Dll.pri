!isEmpty(UNIZ_PRI_INCLUDED):error("Uniz.pri already included")
UNIZ_PRI_INCLUDED = 1

UNIZ_VERSION = 1.0.0 #版本号
UNIZ_COMPAT_VERSION = 1.0.0

CONFIG += c++11


defineReplace(qtLibraryName) {  #生成的库的名字
   unset(LIBRARY_NAME)
   LIBRARY_NAME = $$1
   CONFIG(debug, debug|release) {
      !debug_and_release|build_pass {
          mac:RET = $$member(LIBRARY_NAME, 0)_debug
              else:win32:RET = $$member(LIBRARY_NAME, 0)d
      }
   }
   isEmpty(RET):RET = $$LIBRARY_NAME
    message(RET_lib_name:$$RET)
   return($$RET)
}

defineReplace(stripSrcDir) {
    return($$relative_path($$absolute_path($$1, $$OUT_PWD), $$_PRO_FILE_PWD_))
}

isEmpty(UNIZ_LIBRARY_BASENAME) {
    UNIZ_LIBRARY_BASENAME = lib
}

UNIZ_SOURCE_TREE = $$PWD
isEmpty(UNIZ_BUILD_TREE) {
    sub_dir = $$_PRO_FILE_PWD_
    message(sub_dir:$$_PRO_FILE_PWD_)
    sub_dir ~= s,^$$re_escape($$PWD),,
    UNIZ_BUILD_TREE = $$clean_path($$OUT_PWD)
    message(1UNIZ_BUILD_TREE:$$UNIZ_BUILD_TREE) # D:/Opensource workspace/dllSystem
    UNIZ_BUILD_TREE ~= s,$$re_escape($$sub_dir)$,,
    message(2UNIZ_BUILD_TREE:$$UNIZ_BUILD_TREE)

}

UNIZ_APP_PATH = $$UNIZ_BUILD_TREE/bin
message(UNIZ_APP_PATH:$$UNIZ_APP_PATH) # D:/Opensource workspace/dllSystem/bin
osx {
    UNIZ_APP_TARGET   = "Uniz"
    UNIZ_APP_BUNDLE = $$UNIZ_APP_PATH/$${UNIZ_APP_TARGET}.app
    # set output path if not set manually
    isEmpty(UNIZ_OUTPUT_PATH): UNIZ_OUTPUT_PATH = $$UNIZ_APP_BUNDLE/Contents
    UNIZ_LIBRARY_PATH =     $$UNIZ_OUTPUT_PATH/Frameworks
    UNIZ_PLUGIN_PATH  =     $$UNIZ_OUTPUT_PATH/PlugIns
    UNIZ_LIBEXEC_PATH =     $$UNIZ_OUTPUT_PATH/Resources
    UNIZ_DATA_PATH    =     $$UNIZ_OUTPUT_PATH/Resources
    UNIZ_BIN_PATH     =       $$UNIZ_OUTPUT_PATH/MacOS
    copydata = 1
    LINK_LIBRARY_PATH = $$UNIZ_APP_BUNDLE/Contents/Frameworks
    LINK_PLUGIN_PATH  = $$UNIZ_APP_BUNDLE/Contents/PlugIns
}else{
    contains(TEMPLATE, vc.*):vcproj = 1
    UNIZ_APP_TARGET   = Uniz
    isEmpty(UNIZ_OUTPUT_PATH): UNIZ_OUTPUT_PATH = $$UNIZ_BUILD_TREE
    message(UNIZ_BUILD_TREE:$$UNIZ_BUILD_TREE) # D:/Opensource workspace/dllSystem
    UNIZ_LIBRARY_PATH  = $$UNIZ_OUTPUT_PATH/$$UNIZ_LIBRARY_BASENAME
    message(UNIZ_LIBRARY_PATH:$$UNIZ_LIBRARY_PATH); #D:/Opensource workspace/dllSystem/lib
    UNIZ_PLUGIN_PATH   = $$UNIZ_LIBRARY_PATH/plugins
    message(UNIZ_PLUGIN_PATH:$$UNIZ_PLUGIN_PATH);#D:/Opensource workspace/dllSystem/lib/plugins
    win32: \
        UNIZ_LIBEXEC_PATH = $$UNIZ_OUTPUT_PATH/bin
    else: \
        UNIZ_LIBEXEC_PATH = $$UNIZ_OUTPUT_PATH/libexec/Uniz
    UNIZ_PLUGIN_BIN = $$UNIZ_LIBEXEC_PATH/plugins
    !isEqual(UNIZ_SOURCE_TREE, $$UNIZ_OUTPUT_PATH):copydata = 1
    LINK_LIBRARY_PATH = $$UNIZ_PLUGIN_PATH
    message(-LINK_LIBRARY_PATH:$$LINK_LIBRARY_PATH)
    LINK_PLUGIN_PATH  = $$LINK_LIBRARY_PATH
    message(-LINK_PLUGIN_PATH:$$LINK_PLUGIN_PATH)
    INSTALL_LIBRARY_PATH = $$UNIZ_PREFIX/$$IDE_LIBRARY_BASENAME/Uniz
    INSTALL_PLUGIN_PATH  = $$INSTALL_LIBRARY_PATH/plugins
    message(-INSTALL_PLUGIN_PATH:$$INSTALL_PLUGIN_PATH)
    win32: \
        INSTALL_LIBEXEC_PATH = $$UNIZ_PREFIX/bin
        message(-INSTALL_LIBEXEC_PATH:$$INSTALL_LIBEXEC_PATH)
    else: \
        INSTALL_LIBEXEC_PATH = $$UNIZ_PREFIX/libexec/Uniz
    INSTALL_DATA_PATH    = $$UNIZ_PREFIX/share/Uniz
    INSTALL_BIN_PATH     = $$UNIZ_PREFIX/bin
    message(-INSTALL_BIN_PATH:$$INSTALL_BIN_PATH)
}

INCLUDEPATH += \
    $$UNIZ_BUILD_TREE/src \
    $$UNIZ_SOURCE_TREE/src/core \
    $$UNIZ_SOURCE_TREE/src/systemexternsion

message(UNIZ_SOURCE_TREE:$$UNIZ_SOURCE_TREE)

UNIZ_PLUGIN_DIRS_FROM_ENVIRONMENT = $$(UNIZ_PLUGIN_DIRS)
UNIZ_PLUGIN_DIRS += $$split(UNIZ_PLUGIN_DIRS_FROM_ENVIRONMENT, $$QMAKE_DIRLIST_SEP)
UNIZ_PLUGIN_DIRS += $$UNIZ_SOURCE_TREE/src/plugins
for(dir, UNIZ_PLUGIN_DIRS) {
    INCLUDEPATH += $$dir
    message(UNIZ_PLUGIN_DIRS:$$dir)
}


LIBS *= -L$$LINK_LIBRARY_PATH
message(---LINK_LIBRARY_PATH:$$LINK_LIBRARY_PATH) #D:/Opensource workspace/dllSystem/lib/plugins
exists($$UNIZ_LIBRARY_PATH): LIBS *= -L$$UNIZ_LIBRARY_PATH



!isEmpty(vcproj) {
    DEFINES += UNIZ_LIBRARY_BASENAME=\"$$UNIZ_LIBRARY_BASENAME\"
    message(UNIZ_LIBRARY_BASENAME--:$$UNIZ_LIBRARY_BASENAME)
} else {
    DEFINES += UNIZ_LIBRARY_BASENAME=\\\"$$UNIZ_LIBRARY_BASENAME\\\"
    message(UNIZ_LIBRARY_BASENAME---:$$UNIZ_LIBRARY_BASENAME)
}

msvc {
    DEFINES += _CRT_SECURE_NO_WARNINGS
    QMAKE_CXXFLAGS_WARN_ON *= -w44996
    QMAKE_LFLAGS_DEBUG += /INCREMENTAL:NO
}

qt {
    contains(QT, core): QT += concurrent
    contains(QT, gui): QT += widgets
}

!isEmpty(UNIZ_PLUGIN_DEPENDS) {
    LIBS *= -L$$UNIZ_PLUGIN_PATH  # plugin path from output directory
    message(UNIZ_PLUGIN_PATH-0:$$UNIZ_PLUGIN_PATH) #
    LIBS *= -L$$LINK_PLUGIN_PATH  # when output path is different from Qt Creator build directory
}

done_plugins =
for(ever) {
    isEmpty(UNIZ_PLUGIN_DEPENDS): \
        break()
    done_plugins += $$UNIZ_PLUGIN_DEPENDS
    for(dep, UNIZ_PLUGIN_DEPENDS) {
        dependencies_file =
        for(dir, UNIZ_PLUGIN_DIRS) {
            exists($$dir/$$dep/$${dep}_dependencies.pri) {
                dependencies_file = $$dir/$$dep/$${dep}_dependencies.pri
                message(2:dependencies_file)
                break()
            }
        }
        isEmpty(dependencies_file): \
            message(3:"Plugin dependency $$dep not found")
        include($$dependencies_file)
        LIBS += -l$$qtLibraryName($$UNIZ_PLUGIN_NAME)
    }
    UNIZ_PLUGIN_DEPENDS = $$unique(UNIZ_PLUGIN_DEPENDS)
    UNIZ_PLUGIN_DEPENDS -= $$unique(done_plugins)
    message(UNIZ_PLUGIN_DEPENDS--0:$$UNIZ_PLUGIN_DEPENDS)
}


done_libs =
for(ever) {
    isEmpty(UNIZ_LIB_DEPENDS): \
        break()
    done_libs += $$UNIZ_LIB_DEPENDS
    for(dep, UNIZ_LIB_DEPENDS) {
        include($$PWD/src/$$dep/$${dep}_dependencies.pri)
        message(123:$$PWD/src/$$dep/$${dep}_dependencies.pri)
        LIBS += -l$$qtLibraryName($$UNIZ_LIB_NAME) # 从依赖库中那libname来引用  qtLibraryName 判断是debug还是release
        message(--UNIZ_LIB_NAME-dep--:$$PWD/src/$$dep/$${dep}_dependencies.pri)
    }
    UNIZ_LIB_DEPENDS = $$unique(UNIZ_LIB_DEPENDS)
    UNIZ_LIB_DEPENDS -= $$unique(done_libs)
}

