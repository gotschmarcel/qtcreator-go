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
    ProjectNode(const Utils::FileName& projectFilePath)
        : ProjectExplorer::ProjectNode(projectFilePath) {}

    QList<ProjectExplorer::ProjectAction> supportedActions(Node*) const override { return {}; }

    bool canAddSubProject(const QString&) const override { return false; }
    bool addSubProjects(const QStringList&) override { return false; }
    bool removeSubProjects(const QStringList&) override { return false; }
    bool addFiles(const QStringList&, QStringList*) override { return false; }
    bool removeFiles(const QStringList&, QStringList*) override { return false; }
    bool deleteFiles(const QStringList&) override { return false; }
    bool renameFile(const QString&, const QString&) override { return false; }
};

} // Internal
} // Go
