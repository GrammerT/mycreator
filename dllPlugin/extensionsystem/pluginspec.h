#ifndef PLUGINSPEC_H
#define PLUGINSPEC_H

namespace TZQExtensionSystem
{

namespace ExtenInternal
{
class PluginSpecPrivate;
}

class Plugin;

class PluginSpec
{
public:
    enum State{
        Invalid,
        Read,
        Resolved,
        Loaded,
        Initialized,
        Running,
        Stopped,
        Deleted
    };

    ~PluginSpec();




private:
    PluginSpec();
    ExtenInternal::PluginSpecPrivate *d;
};

}
#endif // PLUGINSPEC_H
