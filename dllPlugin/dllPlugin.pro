include(dllPlugin.pri)

#version check qt
!minQtVersion(5, 5, 0) {
    message("Cannot build dllPlugin with Qt version $${QT_VERSION}.")
    error("Use at least Qt 5.5.0.")
}


TEMPLATE = subdirs
CONFIG += ordered

SUBDIRS = extensionsystem plugins app

contains(QT_ARCH, i386): ARCHITECTURE = x86
else: ARCHITECTURE = $$QT_ARCH


macx: PLATFORM = "mac"
else:win32: PLATFORM = "windows"
else:linux-*: PLATFORM = "linux-$${ARCHITECTURE}"
else: PLATFORM = "unknown"

BASENAME = $$(INSTALL_BASENAME)
isEmpty(BASENAME): BASENAME = dll-plugin-$${PLATFORM}$(INSTALL_EDITION)-$${DLLPLUGIN_VERSION}$(INSTALL_POSTFIX)

macx:INSTALLER_NAME = "dll-plugin-$${DLLPLUGIN_VERSION}"
else:INSTALLER_NAME = "$${BASENAME}"


macx {
    APPBUNDLE = "$$OUT_PWD/bin/Dll Plugins.app"
    BINDIST_SOURCE = "$$OUT_PWD/bin/Dll Plugins.app"
    BINDIST_INSTALLER_SOURCE = $$BINDIST_SOURCE
#    deployqt.commands = $$PWD/scripts/deployqtHelper_mac.sh \"$${APPBUNDLE}\" \"$$[QT_INSTALL_TRANSLATIONS]\" \"$$[QT_INSTALL_PLUGINS]\" \"$$[QT_INSTALL_IMPORTS]\" \"$$[QT_INSTALL_QML]\"
#    codesign.commands = codesign --deep -s \"$(SIGNING_IDENTITY)\" $(SIGNING_FLAGS) \"$${APPBUNDLE}\"
#    dmg.commands = $$PWD/scripts/makedmg.sh $$OUT_PWD/bin $${BASENAME}.dmg
#    QMAKE_EXTRA_TARGETS += codesign dmg
} else {
    BINDIST_SOURCE = "$(INSTALL_ROOT)$$QTC_PREFIX"
    BINDIST_INSTALLER_SOURCE = "$$BINDIST_SOURCE/*"
#    deployqt.commands = python -u $$PWD/scripts/deployqt.py -i \"$(INSTALL_ROOT)$$QTC_PREFIX\" \"$(QMAKE)\"
#    deployqt.depends = install
    win32 {
#        deployartifacts.depends = install
#        deployartifacts.commands = git clone "git://code.qt.io/qt-creator/binary-artifacts.git" -b $$BINARY_ARTIFACTS_BRANCH&& xcopy /s /q /y /i "binary-artifacts\\win32" \"$(INSTALL_ROOT)$$QTC_PREFIX\"&& rmdir /s /q binary-artifacts
#        QMAKE_EXTRA_TARGETS += deployartifacts
    }
}
