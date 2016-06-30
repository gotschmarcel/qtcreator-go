#pragma once

#include <QDir>

namespace Go {
namespace Internal {

class Toolchain {
public:
    static QDir goPath();
};

}
}
