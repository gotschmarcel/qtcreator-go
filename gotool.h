#pragma once

#include <coreplugin/icore.h>
#include <utils/fileutils.h>

#include "go_global.h"

namespace Go {
namespace Internal {

class GOSHARED_EXPORT GoTool
{
public:
    enum class Detection
    {
        AUTO,
        MANUAL
    };

    explicit GoTool(const Detection &detection, const Utils::FileName &executablePath);
    explicit GoTool(const Detection &detection);

    bool isAutodetected() const;
    Core::Id id() const;
    Utils::FileName executablePath() const;

private:
    Q_DISABLE_COPY(GoTool)

    Detection _detection;
    Utils::FileName _executablePath;
    Core::Id _id;
};

} // namespace Internal
} // namespace Go
