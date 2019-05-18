include($$replace(_PRO_FILE_PWD_),([^/]+$), \\1/\\1_dependencies.pri))
TARGET = $$DLLP_LIB_NAME

include(dllPlugin.pri)

win32{
    DLLDESTDIR = $$IDE_APP_PATH
}

DESTDIR = $$IDE_LIBRARY_PATH
message(-----------$$IDE_LIBRARY_PATH)

TARGET = $$dllLibraryName($$TARGET)

TEMPLATE = lib
CONFIG += shared dll

contains(DLLP_CONFIG, reduce_exports):CONFIG += hide_symbols

!macx {
    win32 {
        dlltarget.path = $$INSTALL_BIN_PATH
        INSTALLS += dlltarget
    } else {
        target.path = $$INSTALL_LIBRARY_PATH
        INSTALLS += target
    }
}
