#pragma once

#include <projectexplorer/projectnodes.h>

#include "../go_global.h"

namespace Utils {
class FileName;
}

namespace Go {
namespace Internal {

class GoProjectNode : public ProjectExplorer::ProjectNode
{
public:
    GoProjectNode(const Utils::FileName &projectFilePath);

    bool supportsAction(ProjectExplorer::ProjectAction action, ProjectExplorer::Node *node) const override;

    bool addFiles(const QStringList &filePaths, QStringList *notAdded = 0) override;
    bool removeFiles(const QStringList &filePaths, QStringList *notRemoved = 0) override;
    bool deleteFiles(const QStringList &filePaths) override;
    bool canRenameFile(const QString &filePath, const QString &newFilePath) override;
    bool renameFile(const QString &filePath, const QString &newFilePath) override;
};

} // namespace Internal
} // namespace Go
