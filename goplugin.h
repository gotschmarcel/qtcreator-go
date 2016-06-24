#pragma once

#include "go_global.h"

#include <extensionsystem/iplugin.h>

namespace Go {
namespace Internal {

class GoPlugin final : public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "Go.json")

public:
    GoPlugin();
    ~GoPlugin();

    bool initialize(const QStringList &arguments, QString *errorString) override;
    void extensionsInitialized() override;
    ShutdownFlag aboutToShutdown() override;
};

} // namespace Internal
} // namespace Go
