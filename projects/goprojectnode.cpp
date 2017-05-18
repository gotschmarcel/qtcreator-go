#include "goprojectnode.h"

#include <QDir>
#include <QStringList>

#include <utils/fileutils.h>

using namespace Go::Internal;

ProjectNode::ProjectNode(const Utils::FileName& projectFilePath)
    : ProjectExplorer::ProjectNode(projectFilePath) {
    setDisplayName(QDir(projectFilePath.toString()).dirName());
}

QList<ProjectExplorer::ProjectAction> ProjectNode::supportedActions(ProjectExplorer::Node* node) const {
    if (node->nodeType() == ProjectExplorer::ProjectNodeType) {
        return {ProjectExplorer::AddNewFile,
                    ProjectExplorer::AddExistingFile,
                    ProjectExplorer::AddExistingDirectory};
    }

    if (node->nodeType() == ProjectExplorer::FolderNodeType) {
        return {ProjectExplorer::AddNewFile,
                    ProjectExplorer::AddExistingFile,
                    ProjectExplorer::AddExistingDirectory,
                    ProjectExplorer::Rename};
    }

    if (node->nodeType() == ProjectExplorer::FileNodeType) {
        return {ProjectExplorer::RemoveFile,
                    ProjectExplorer::Rename,
                    ProjectExplorer::DuplicateFile};
    }

    return {};
}

bool ProjectNode::addFiles(const QStringList& filePaths, QStringList* notAdded)
{
    Q_UNUSED(filePaths);
    Q_UNUSED(notAdded);
    return true;
}

bool ProjectNode::removeFiles(const QStringList& filePaths, QStringList* notRemoved)
{
    Q_UNUSED(filePaths);
    Q_UNUSED(notRemoved);
    return true;
}

bool ProjectNode::deleteFiles(const QStringList& filePaths)
{
    Q_UNUSED(filePaths);
    return true;
}

bool ProjectNode::canRenameFile(const QString& filePath, const QString& newFilePath)
{
    Q_UNUSED(filePath);
    Q_UNUSED(newFilePath);
    return true;
}

bool ProjectNode::renameFile(const QString& filePath, const QString& newFilePath)
{
    Q_UNUSED(filePath);
    Q_UNUSED(newFilePath);
    return true;
}
