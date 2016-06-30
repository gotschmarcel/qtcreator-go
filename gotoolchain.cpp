#include "gotoolchain.h"

#include <QProcessEnvironment>

#include "goconstants.h"
#include "gomessages.h"

using namespace Go::Internal;

QDir Toolchain::goPath() {
    const auto& env = QProcessEnvironment::systemEnvironment();

    if (!env.contains(QLatin1String(Constants::GoPathEnvName))) {
        Messages::warning() << "GOPATH not set in environment."
                            << "Code completion and other features will not work.";
    }

    return env.value(QLatin1String(Constants::GoPathEnvName));
}
