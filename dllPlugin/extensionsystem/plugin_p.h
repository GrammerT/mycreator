#ifndef PLUGIN_P_H
#define PLUGIN_P_H

#include "plugin.h"

namespace TZQExtensionSystem
{

class PluginManager;
class PluginSpec;

namespace ExtenInternal
{

class PluginPrivate
{
public:
    PluginSpec *pluginSpec;
};

}//! internal
}//! extensionSystem

#endif // PLUGIN_P_H
