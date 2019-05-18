#include "coreplugin.h"
#include "mainwindow.h"

using namespace TZQCore;
using namespace TZQCore::CoreInternal;


CorePlugin::CorePlugin()
    :m_mainWindow(0)
{


}

CorePlugin::~CorePlugin()
{
    delete m_mainWindow;
}

bool CorePlugin::initialize()
{
    initCorePlugin();
    return true;
}

void CorePlugin::initCorePlugin()
{
     m_mainWindow = new MainWindow;
}

