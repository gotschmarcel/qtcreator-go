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

class ProjectManager;
class ProjectNode;

class GOSHARED_EXPORT Project final : public ProjectExplorer::Project {
    Q_OBJECT

public:
    static const int kMinTimeBetweenScans = 4500;

    Project(ProjectManager* manager, const QString& fileName);
    QString displayName() const override;

    QStringList files(FilesMode) const override;

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

    void recursiveScanDirectory(const QDir& dir, QSet<QString>& container);
    void addNodes(const QSet<QString>& nodes);
    void removeNodes(const QSet<QString>& nodes);
    void tryRemoveEmptyFolder(ProjectExplorer::FolderNode* folder);

    ProjectExplorer::FolderNode* findFolderForRelPath(const QString& relPath);
};

} // Internal
} // Go
