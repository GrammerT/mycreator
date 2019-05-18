#ifndef PLUGINSPEC_P_H
#define PLUGINSPEC_P_H
#include "pluginspec.h"
#include <QObject>

namespace TZQExtensionSystem
{
class Plugin;
class PluginManager;

namespace ExtenInternal
{

class PluginSpecPrivate :public QObject
{
    Q_OBJECT
public:
    PluginSpecPrivate(PluginSpec *spec);

    QString name();
    QString version();
    bool required;

    PluginSpec::State state;

private:
    PluginSpec *q;
};
}//! internal
}//! extensionSystem



#endif // PLUGINSPEC_P_H
