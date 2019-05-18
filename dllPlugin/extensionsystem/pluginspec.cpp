#include "pluginspec.h"
#include "pluginspec_p.h"

using namespace TZQExtensionSystem;
using namespace TZQExtensionSystem::ExtenInternal;

PluginSpec::~PluginSpec()
{
    delete d;
    d=0;
}

PluginSpec::PluginSpec()
    :d(new PluginSpecPrivate(this))
{

}

PluginSpecPrivate::PluginSpecPrivate(PluginSpec *spec)
    :q(spec)
{

}
