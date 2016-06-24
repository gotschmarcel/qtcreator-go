#pragma once

#include <projectexplorer/iprojectmanager.h>

namespace Go {

class ProjectManager final : public ProjectExplorer::IProjectManager
{
    Q_OBJECT

public:
    QString mimeType() const override;
    ProjectExplorer::Project* openProject(const QString &fileName, QString *errorString) override;
};

} // Go
