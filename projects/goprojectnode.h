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

    QList<ProjectExplorer::ProjectAction> supportedActions(Node*node) const override;

    bool addFiles(const QStringList &filePaths, QStringList *notAdded = 0) override;
    bool removeFiles(const QStringList &filePaths, QStringList *notRemoved = 0) override;
    bool deleteFiles(const QStringList &filePaths) override;
    bool canRenameFile(const QString &filePath, const QString &newFilePath) override;
    bool renameFile(const QString &filePath, const QString &newFilePath) override;
};

} // Internal
} // Go
