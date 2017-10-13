#include "gotoolmanager.h"

#include <QFileInfo>
#include <QStringList>

#include <utils/environment.h>

#include "goconstants.h"
#include "gotool.h"

namespace Go {
namespace Internal {

void GoToolManager::autoDetectTools(const QStringList &additionalLocations)
{
    Utils::FileNameList suspects;

    auto env = Utils::Environment::systemEnvironment();
    auto path = env.path();

    // Check GOROOT
    if (env.hasKey(QLatin1String(Constants::GoRootEnvName))) {
        const QString &goRoot = env.value(QLatin1String(Constants::GoRootEnvName));
        path.append(goRoot);
    } else {
        path.append(Constants::DefaultGoRoot);
    }

    path.append(additionalLocations);
    path.removeDuplicates();

    auto execs = env.appendExeExtensions(QLatin1String(Constants::GoExecName));

    for (auto p : path) {
        if (p.isEmpty()) {
            continue;
        }

        // Append "/" if necessary.
        const QChar slash = QLatin1Char('/');
        if (!p.endsWith(slash)) {
            p += slash;
        }

        for (const auto &e : execs) {
            QFileInfo info(p + e);

            if (info.exists() && info.isFile() && info.isExecutable()) {
                suspects << Utils::FileName::fromString(info.absoluteFilePath());
            }
        }
    }

    for (const auto &exe : suspects) {
        _tools << new GoTool(GoTool::Detection::AUTO, exe);
    }
}

const QList<GoTool *> &GoToolManager::tools() const { return _tools; }

GoToolManager &GoToolManager::instance()
{
    static GoToolManager m;
    return m;
}

GoToolManager::GoToolManager() {}

GoToolManager::~GoToolManager()
{
    qDeleteAll(_tools);
    _tools.clear();
}

} // Internal
} // Go
