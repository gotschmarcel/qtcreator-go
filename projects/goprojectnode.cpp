#include "goprojectnode.h"

#include <QDir>
#include <QStringList>

#include <utils/fileutils.h>

using namespace Go::Internal;

ProjectNode::ProjectNode(const Utils::FileName& projectFilePath)
    : ProjectExplorer::ProjectNode(projectFilePath) {}

QList<ProjectExplorer::ProjectAction> ProjectNode::supportedActions(ProjectExplorer::Node*) const {
    return {ProjectExplorer::AddNewFile, ProjectExplorer::RemoveFile, ProjectExplorer::Rename};
}

bool ProjectNode::addFiles(const QStringList&, QStringList*) { return true; }

bool ProjectNode::removeFiles(const QStringList&, QStringList*) { return true; }

bool ProjectNode::deleteFiles(const QStringList&) { return true; }

bool ProjectNode::renameFile(const QString&, const QString&) { return true; }
