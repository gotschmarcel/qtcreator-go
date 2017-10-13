#include "goprojectnode.h"

#include <QDir>
#include <QStringList>

#include <utils/fileutils.h>

namespace Go {
namespace Internal {

GoProjectNode::GoProjectNode(const Utils::FileName &projectFilePath)
    : ProjectExplorer::ProjectNode(projectFilePath)
{
    setDisplayName(QDir(projectFilePath.toString()).dirName());
}

bool GoProjectNode::supportsAction(ProjectExplorer::ProjectAction action, ProjectExplorer::Node* node) const
{
    switch (node->nodeType()) {
    case ProjectExplorer::NodeType::Project:
    case ProjectExplorer::NodeType::Folder:
       return action == ProjectExplorer::AddNewFile
           || action == ProjectExplorer::AddExistingFile
           || action == ProjectExplorer::AddExistingDirectory;
    case ProjectExplorer::NodeType::File:
        return action == ProjectExplorer::RemoveFile
            || action == ProjectExplorer::Rename
            || action == ProjectExplorer::DuplicateFile;
    default:
        break;
    }
    return false;
}

bool GoProjectNode::addFiles(const QStringList &filePaths, QStringList *notAdded)
{
    Q_UNUSED(filePaths);
    Q_UNUSED(notAdded);
    return true;
}

bool GoProjectNode::removeFiles(const QStringList &filePaths, QStringList *notRemoved)
{
    Q_UNUSED(filePaths);
    Q_UNUSED(notRemoved);
    return true;
}

bool GoProjectNode::deleteFiles(const QStringList &filePaths)
{
    Q_UNUSED(filePaths);
    return true;
}

bool GoProjectNode::canRenameFile(const QString &filePath, const QString &newFilePath)
{
    Q_UNUSED(filePath);
    Q_UNUSED(newFilePath);
    return true;
}

bool GoProjectNode::renameFile(const QString &filePath, const QString &newFilePath)
{
    Q_UNUSED(filePath);
    Q_UNUSED(newFilePath);
    return true;
}

} // Internal
} // Go
