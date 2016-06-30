#pragma once

#include <coreplugin/idocument.h>

#include "../go_global.h"

namespace Utils {
class FileName;
}

namespace Go {
namespace Internal {

class GOSHARED_EXPORT GoFile final : public Core::IDocument {
public:
    GoFile(const Utils::FileName& fileName);
    ReloadBehavior reloadBehavior(ChangeTrigger, ChangeType) const override;
};

} // Internal
} // Go
