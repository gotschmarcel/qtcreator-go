#include "gotoolchain.h"

#include <QProcessEnvironment>

#include "goconstants.h"

namespace Go {
namespace Internal {

QString GoToolchain::goPath()
{
    return QProcessEnvironment::systemEnvironment().value(QLatin1String(Constants::GoPathEnvName));
}

} // Internal
} // Go
