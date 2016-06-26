#include "goproject.h"

#include <QRegExp>

#include "../goconstants.h"
#include "goprojectmanager.h"
#include "goprojectnode.h"
#include "gofile.h"

using namespace Go;

Project::Project(ProjectManager *manager, const QString &fileName)
    : _manager(manager),
      _rootNode(nullptr)
{
    _dir        = QFileInfo(fileName).dir();
    _rootNode   = new ProjectNode(Utils::FileName::fromString(_dir.dirName()));

    _scanTimer.setSingleShot(true);
    connect(&_scanTimer, SIGNAL(timeout()), SLOT(populateProject()));

    populateProject();

    connect(&_fsWatcher, SIGNAL(directoryChanged(QString)), SLOT(populateProject()));

    setDocument(new GoFile(Utils::FileName::fromString(fileName)));
    setRootProjectNode(_rootNode);
}

QString Project::displayName() const
{
    return _dir.dirName();
}

ProjectExplorer::IProjectManager* Project::projectManager() const
{
    return _manager;
}

ProjectExplorer::ProjectNode* Project::rootProjectNode() const
{
    return _rootNode;
}

QStringList Project::files(Project::FilesMode) const
{
    return _files.toList();
}

void Project::scheduleProjectScan()
{
    const auto elapsed = _lastScan.elapsed();

    if (elapsed < kMinTimeBetweenScans) {
        if (_scanTimer.isActive()) {
            _scanTimer.setInterval(kMinTimeBetweenScans - elapsed);
            _scanTimer.start();
        }

        return;
    }

    populateProject();
}

void Project::populateProject()
{
    _lastScan.start();

    QSet<QString> oldFiles(_files);
    _files.clear();

    recursiveScanDirectory(_dir, _files);

    const auto removedFiles = oldFiles - _files;
    const auto addedFiles   = _files - oldFiles;

    removeNodes(removedFiles);
    addNodes(addedFiles);

    if (removedFiles.size() || addedFiles.size()) {
        emit fileListChanged();
    }
}

void Project::recursiveScanDirectory(const QDir &dir, QSet<QString> &container)
{
    QRegExp projectFilePattern(
                QLatin1String(".*\\") +
                QLatin1String(Constants::ProjectFileExt) +
                QLatin1String("(?:\\.user)?$"));

    for (const auto& info : dir.entryInfoList(
             QDir::AllDirs |
             QDir::Files |
             QDir::NoDotAndDotDot |
             QDir::NoSymLinks |
             QDir::CaseSensitive)) {

        if (info.isDir()) {
            recursiveScanDirectory(info.filePath(), container);
        } else if (projectFilePattern.indexIn(info.fileName()) == -1) {
            container << info.filePath();
        }
    }

    _fsWatcher.addPath(dir.absolutePath());
}

void Project::addNodes(const QSet<QString> &nodes)
{
    for (const auto& node : nodes) {
        const auto info = QFileInfo(_dir.relativeFilePath(node));

        auto folder = findFolderForRelPath(info.filePath());
        folder->addFileNodes(
                    QList<ProjectExplorer::FileNode*>() << new ProjectExplorer::FileNode(
                        Utils::FileName::fromString(node),
                        ProjectExplorer::SourceType,
                        false));
    }
}

void Project::removeNodes(const QSet<QString> &nodes)
{
    for (const auto& node : nodes) {
        const auto info = QFileInfo(_dir.relativeFilePath(node));

        auto folder = findFolderForRelPath(info.filePath());

        for (const auto file : folder->fileNodes()) {
            if (file->filePath().toString() != node) {
                continue;
            }

            folder->removeFileNodes({ file });
        }
    }
}

void Project::tryRemoveEmptyFolder(ProjectExplorer::FolderNode* folder)
{
    if (folder == _rootNode) {
        return;
    }

    auto parent = folder->parentFolderNode();
    if (!parent || !folder->fileNodes().empty() || !folder->subFolderNodes().empty()) {
        return;
    }

    parent->removeFolderNodes({ folder });
    tryRemoveEmptyFolder(parent);
}

ProjectExplorer::FolderNode* Project::findFolderForRelPath(const QString& relPath)
{
    ProjectExplorer::FolderNode *folder = _rootNode;

    auto segments = relPath.split(QLatin1Char('/'));
    segments.removeLast(); // Ignore the file name itself.

    for (const auto& segment : segments) {
        bool found = false;

        for (auto subFolder : folder->subFolderNodes()) {
            if (subFolder->displayName() != segment) {
                continue;
            }

            folder = subFolder;
            found = true;

            break;
        }

        if (!found) {
            auto newFolder = new ProjectExplorer::FolderNode(Utils::FileName::fromString(segment));
            folder->addFolderNodes({ newFolder });
            folder = newFolder;
        }
    }

    return folder;
}
