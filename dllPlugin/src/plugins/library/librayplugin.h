#ifndef LIBRAYPLUGIN_H
#define LIBRAYPLUGIN_H
#include "iplugin.h"

namespace Library {

class LibrayPlugin:public Systemexternsion::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.uniz-project.uniz.UnizPlugin" FILE "library.json")
public:
    LibrayPlugin();
    virtual bool initialize(const QStringList &arguments, QString *errorMessage) override;

    virtual void extensionInitialized() override;
};

}
#endif // LIBRAYPLUGIN_H
