#include <QApplication>
#include "pluginmanager.h"

using namespace TZQExtensionSystem;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    PluginManager pluginManager;

    const QString pluginPaths = ;

    PluginManager::setPluginPaths();

    return a.exec();
}
