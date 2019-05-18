#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include <QObject>

namespace TZQExtensionSystem
{

namespace ExtenInternal {
class PluginManagerPrivate;
}

class Plugin;

class PluginManager : public QObject
{
    Q_OBJECT
public:
    static PluginManager *instance();
    PluginManager();

    //! plugin operations
    static void loadPlugins();
    static void setPluginPaths(const QStringList &paths);
    //! end plugin opertations

};
}

#endif // PLUGINMANAGER_H
