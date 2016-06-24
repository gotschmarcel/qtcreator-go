#pragma once

#include <projectexplorer/project.h>
#include <projectexplorer/projectnodes.h>

#include <QTimer>
#include <QElapsedTimer>
#include <QFileSystemWatcher>
#include <QSet>
#include <QDir>

namespace Go {

class ProjectManager;
class ProjectNode;

class Project final : public ProjectExplorer::Project
{
    Q_OBJECT

public:
    static const int kMinTimeBetweenScans = 4500;

    Project(ProjectManager *manager, const QString& fileName);
    QString displayName() const override;
    ProjectExplorer::IProjectManager* projectManager() const override;
    ProjectExplorer::ProjectNode* rootProjectNode() const override;

    QStringList files(FilesMode) const override;

private slots:
    void scheduleProjectScan();
    void populateProject();

private:
    ProjectManager*     _manager;
    ProjectNode*        _rootNode;

    QDir                _dir;
    QSet<QString>       _files;
    QFileSystemWatcher  _fsWatcher;

    QElapsedTimer       _lastScan;
    QTimer              _scanTimer;

    void recursiveScanDirectory(const QDir& dir, QSet<QString> &container);
    void addNodes(const QSet<QString>& nodes);
    void removeNodes(const QSet<QString>& nodes);
    void tryRemoveEmptyFolder(ProjectExplorer::FolderNode* folder);

    ProjectExplorer::FolderNode* findFolderForRelPath(const QString& relPath);
};

} // Go
