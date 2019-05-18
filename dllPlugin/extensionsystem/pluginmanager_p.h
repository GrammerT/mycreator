#ifndef PLUGINMANAGER_P_H
#define PLUGINMANAGER_P_H
#include <QObject>
#include <QMap>
#include "pluginspec.h"

namespace TZQExtensionSystem {

class PluginManager;

namespace ExtenInternal {
class PluginSpecPrivate;

class PluginManagerPrivate:QObject
{
    Q_OBJECT
public:
    PluginManagerPrivate(PluginManager *manager);
    virtual ~PluginManagerPrivate();

    void loadPlugins();
    void setPluginPaths(const QStringList &paths);

    QList<PluginSpec*> pluginSpecs;

    QStringList pluginPaths;

private:
    PluginManager *q;
    QMap<QString,QMetaObject> nameType;
};

}//! internal
}//! extension system



#endif // PLUGINMANAGER_P_H
