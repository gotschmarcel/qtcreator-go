#pragma once

#include <projectexplorer/projectnodes.h>

#include "../go_global.h"

namespace Utils {
class FileName;
}

namespace Go {
namespace Internal {

class GOSHARED_EXPORT ProjectNode final : public ProjectExplorer::ProjectNode {
public:
    ProjectNode(const Utils::FileName& projectFilePath);

    QList<ProjectExplorer::ProjectAction> supportedActions(Node*) const override;

    bool addFiles(const QStringList&, QStringList* = nullptr) override;
    bool removeFiles(const QStringList&, QStringList* = nullptr) override;
    bool deleteFiles(const QStringList&) override;
    bool renameFile(const QString&, const QString&) override;
};

} // Internal
} // Go
