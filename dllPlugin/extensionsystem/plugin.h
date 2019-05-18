#ifndef PLUGIN_H
#define PLUGIN_H

#include <QObject>

namespace TZQExtensionSystem {

namespace ExtenInternal{
class PluginPrivate;
}

class Plugin : public QObject
{
    Q_OBJECT
public:
    Plugin();
    virtual ~Plugin();
    virtual bool initialize()=0;

private:
    ExtenInternal::PluginPrivate *d;
};

}//! extensiong system

#endif // PLUGIN_H
