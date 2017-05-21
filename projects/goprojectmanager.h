#pragma once

#include <projectexplorer/iprojectmanager.h>

#include "../go_global.h"

namespace Go {
namespace Internal {

class GOSHARED_EXPORT ProjectManager final : public ProjectExplorer::IProjectManager
{
    Q_OBJECT

public:
    QString mimeType() const override;
    ProjectExplorer::Project *openProject(const QString &fileName, QString *errorString) override;
};

} // namespace Internal
} // namespace Go
