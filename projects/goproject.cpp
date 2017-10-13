#include "goproject.h"

#include <coreplugin/documentmanager.h>
#include <coreplugin/icontext.h>

#include <QRegExp>

#include "../goconstants.h"
#include "../generalmessages.h"
#include "../gotoolchain.h"
#include "goprojectfile.h"
#include "goprojectnode.h"

namespace Go {
namespace Internal {

GoProject::GoProject(const Utils::FileName& fileName)
    : ProjectExplorer::Project(Constants::ProjectMIMEType, fileName)
{
    _dir = QFileInfo(fileName.toString()).dir();

    setId(Constants::ProjectID);
    setRootProjectNode(new GoProjectNode(Utils::FileName::fromString(_dir.dirName())));

    Core::DocumentManager::addDocument(document());

    _scanTimer.setSingleShot(true);
    connect(&_scanTimer, SIGNAL(timeout()), SLOT(populateProject()));

    populateProject();

    connect(&_fsWatcher, SIGNAL(directoryChanged(QString)), SLOT(populateProject()));

    if (GoToolchain::goPath().isEmpty()) {
        GeneralMessages::warning()
            << "GOPATH not set. Autocomplete and other features will not work!";
    }
}

bool GoProject::requiresTargetPanel() const { return !targets().isEmpty(); }

void GoProject::scheduleProjectScan()
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

void GoProject::populateProject()
{
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

void GoProject::recursiveScanDirectory(const QDir &dir, QSet<QString> &container)
{
    for (const auto &info : dir.entryInfoList(QDir::AllDirs | QDir::Files | QDir::NoDotAndDotDot
                                              | QDir::NoSymLinks | QDir::CaseSensitive)) {

        if (info.isDir()) {
            recursiveScanDirectory(info.filePath(), container);
        } else if (info.suffix() == Constants::FileExt) {
            container << info.filePath();
        }
    }

    _fsWatcher.addPath(dir.absolutePath());
}

void GoProject::addNodes(const QSet<QString> &nodes)
{
    for (const auto &node : nodes) {
        const auto info = QFileInfo(_dir.relativeFilePath(node));
        auto folder = findFolderForRelPath(info.filePath());
        folder->addNestedNode(new ProjectExplorer::FileNode{Utils::FileName::fromString(node), ProjectExplorer::FileType::Source, false});
    }
}

void GoProject::removeNodes(const QSet<QString> &nodes)
{
    for (const auto &node : nodes) {
        const auto info = QFileInfo(_dir.relativeFilePath(node));

        auto folder = findFolderForRelPath(info.filePath());

        for (const auto file : folder->fileNodes()) {
            if (file->filePath().toString() != node) {
                continue;
            }

            folder->removeNode(file);
        }

        tryRemoveEmptyFolder(folder);
    }
}

void GoProject::tryRemoveEmptyFolder(ProjectExplorer::FolderNode *folder)
{
    if (folder == rootProjectNode()) {
        return;
    }

    auto parent = folder->parentFolderNode();
    if (!parent || !folder->fileNodes().empty() || !folder->folderNodes().empty()) {
        return;
    }

    parent->removeNode(folder);
    tryRemoveEmptyFolder(parent);
}

ProjectExplorer::FolderNode *GoProject::findFolderForRelPath(const QString &relPath)
{
    ProjectExplorer::FolderNode *folder = rootProjectNode();

    auto segments = relPath.split(QLatin1Char('/'));
    segments.removeLast(); // Ignore the file name itself.

    for (const auto &segment : segments) {
        bool found = false;

        for (auto subFolder : folder->folderNodes()) {
            if (subFolder->displayName() != segment) {
                continue;
            }

            folder = subFolder;
            found = true;

            break;
        }

        if (!found) {
            auto newFolder = new ProjectExplorer::FolderNode(Utils::FileName::fromString(segment));
            folder->addNode(newFolder);
            folder = newFolder;
        }
    }

    return folder;
}

} // Internal
} // Go
