#include "goproject.h"

#include <coreplugin/documentmanager.h>
#include <coreplugin/icontext.h>

#include <QRegExp>

#include "../goconstants.h"
#include "../gotoolchain.h"
#include "../gogeneralmessages.h"
#include "goprojectfile.h"
#include "goprojectmanager.h"
#include "goprojectnode.h"

using namespace Go::Internal;

Project::Project(ProjectManager* manager, const QString& fileName) {
    _dir = QFileInfo(fileName).dir();

    setId(Constants::ProjectID);
    setProjectContext(Core::Context(Constants::ProjectContext));
    setProjectManager(manager);
    setRootProjectNode(new ProjectNode(Utils::FileName::fromString(_dir.dirName())));
    setDocument(new GoProjectFile(Utils::FileName::fromString(fileName)));

    Core::DocumentManager::addDocument(document());

    _scanTimer.setSingleShot(true);
    connect(&_scanTimer, SIGNAL(timeout()), SLOT(populateProject()));

    populateProject();

    connect(&_fsWatcher, SIGNAL(directoryChanged(QString)), SLOT(populateProject()));

    if (Toolchain::goPath().isEmpty()) {
        GeneralMessages::warning() << "GOPATH not set. Autocomplete and other features will not work!";
    }
}

QString Project::displayName() const { return _dir.dirName(); }

QStringList Project::files(Project::FilesMode) const { return _files.toList(); }

bool Project::requiresTargetPanel() const { return !targets().isEmpty(); }

void Project::scheduleProjectScan() {
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

void Project::populateProject() {
    _lastScan.start();

    QSet<QString> oldFiles(_files);
    _files.clear();

    recursiveScanDirectory(_dir, _files);

    const auto removedFiles = oldFiles - _files;
    const auto addedFiles = _files - oldFiles;

    removeNodes(removedFiles);
    addNodes(addedFiles);

    if (removedFiles.size() || addedFiles.size()) {
        emit fileListChanged();
    }
}

void Project::recursiveScanDirectory(const QDir& dir, QSet<QString>& container) {
    for (const auto& info : dir.entryInfoList(QDir::AllDirs | QDir::Files | QDir::NoDotAndDotDot |
                                              QDir::NoSymLinks | QDir::CaseSensitive)) {

        if (info.isDir()) {
            recursiveScanDirectory(info.filePath(), container);
        } else if (info.suffix() == Constants::FileExt) {
            container << info.filePath();
        }
    }

    _fsWatcher.addPath(dir.absolutePath());
}

void Project::addNodes(const QSet<QString>& nodes) {
    for (const auto& node : nodes) {
        const auto info = QFileInfo(_dir.relativeFilePath(node));

        auto folder = findFolderForRelPath(info.filePath());
        folder->addFileNodes(
            QList<ProjectExplorer::FileNode*>() << new ProjectExplorer::FileNode(
                Utils::FileName::fromString(node), ProjectExplorer::SourceType, false));
    }
}

void Project::removeNodes(const QSet<QString>& nodes) {
    for (const auto& node : nodes) {
        const auto info = QFileInfo(_dir.relativeFilePath(node));

        auto folder = findFolderForRelPath(info.filePath());

        for (const auto file : folder->fileNodes()) {
            if (file->filePath().toString() != node) {
                continue;
            }

            folder->removeFileNodes({file});
        }

        tryRemoveEmptyFolder(folder);
    }
}

void Project::tryRemoveEmptyFolder(ProjectExplorer::FolderNode* folder) {
    if (folder == rootProjectNode()) {
        return;
    }

    auto parent = folder->parentFolderNode();
    if (!parent || !folder->fileNodes().empty() || !folder->subFolderNodes().empty()) {
        return;
    }

    parent->removeFolderNodes({folder});
    tryRemoveEmptyFolder(parent);
}

ProjectExplorer::FolderNode* Project::findFolderForRelPath(const QString& relPath) {
    ProjectExplorer::FolderNode* folder = rootProjectNode();

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
            folder->addFolderNodes({newFolder});
            folder = newFolder;
        }
    }

    return folder;
}
