#ifndef COREPLUGIN_H
#define COREPLUGIN_H
#include "plugin.h"

namespace TZQCore
{

namespace CoreInternal
{

class MainWindow;


class CorePlugin : public TZQExtensionSystem::Plugin
{
    Q_OBJECT
public:
    Q_INVOKABLE CorePlugin();
    ~CorePlugin();
    virtual bool initialize() override;

private:
    void initCorePlugin();


private:
    MainWindow *m_mainWindow;
};


}//! TZQInternal
}//! TZQCore
#endif // COREPLUGIN_H
