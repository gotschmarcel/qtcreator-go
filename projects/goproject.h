#pragma once

#include <projectexplorer/project.h>
#include <projectexplorer/projectnodes.h>

#include <QDir>
#include <QElapsedTimer>
#include <QFileSystemWatcher>
#include <QSet>
#include <QTimer>

#include "../go_global.h"

namespace Go {
namespace Internal {

class GOProjectManager;
class GoProjectNode;

class GoProject : public ProjectExplorer::Project
{
    Q_OBJECT

public:
    static const int kMinTimeBetweenScans = 4500;

    GoProject(const Utils::FileName& fileName);
    bool requiresTargetPanel() const override;

private slots:
    void scheduleProjectScan();
    void populateProject();

private:
    QDir _dir;
    QSet<QString> _files;
    QFileSystemWatcher _fsWatcher;

    QElapsedTimer _lastScan;
    QTimer _scanTimer;

    void recursiveScanDirectory(const QDir &dir, QSet<QString> &container);
    void addNodes(const QSet<QString> &nodes);
    void removeNodes(const QSet<QString> &nodes);
    void tryRemoveEmptyFolder(ProjectExplorer::FolderNode *folder);

    ProjectExplorer::FolderNode *findFolderForRelPath(const QString &relPath);
};

} // namespace Internal
} // namespace Go
