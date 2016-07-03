#include "gotoolchain.h"

#include <QProcessEnvironment>

#include "goconstants.h"

using namespace Go::Internal;

QString Toolchain::goPath() {
    return QProcessEnvironment::systemEnvironment().value(QLatin1String(Constants::GoPathEnvName));
}
