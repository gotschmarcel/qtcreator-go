#include "goprojectmanager.h"

#include <QFileInfo>

#include "../goconstants.h"
#include "goproject.h"

using namespace Go::Internal;

QString ProjectManager::mimeType() const
{
    return QLatin1String(Constants::ProjectMIMEType);
}

ProjectExplorer::Project *ProjectManager::openProject(const QString &fileName, QString *errorString)
{
    if (!QFileInfo(fileName).isFile()) {
        *errorString = tr("Failed opening project '%1': Not a file").arg(fileName);
        return nullptr;
    }

    return new Project(this, fileName);
}
