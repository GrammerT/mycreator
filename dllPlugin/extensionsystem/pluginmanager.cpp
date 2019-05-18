#include "pluginmanager.h"
#include "pluginmanager_p.h"
#include <QList>
#include <QMap>

using namespace TZQExtensionSystem;
using namespace TZQExtensionSystem::ExtenInternal;

static PluginManager *m_instance = 0;
static ExtenInternal::PluginManagerPrivate *d=0;




PluginManager *PluginManager::instance()
{
    return m_instance;
}

PluginManager::PluginManager()
{
    m_instance = this;
    d = new PluginManagerPrivate(this);
}

void PluginManager::loadPlugins()
{
    return d->loadPlugins();
}

void PluginManager::setPluginPaths(const QStringList &paths)
{
    return d->setPluginPaths(paths);
}


PluginManagerPrivate::PluginManagerPrivate(PluginManager *manager)
    :q(manager)
{

}

PluginManagerPrivate::~PluginManagerPrivate()
{

}

void PluginManagerPrivate::loadPlugins()
{
    QMap<QString,QMetaObject>::iterator iter;
    for(iter=nameType.begin();iter!=nameType.end();++iter)
    {

    }
}

void PluginManagerPrivate::setPluginPaths(const QStringList &paths)
{
    pluginPaths = paths;
}
