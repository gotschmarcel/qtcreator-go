#pragma once

#include <coreplugin/idocument.h>

#include "../go_global.h"

namespace Utils {
class FileName;
}

namespace Go {

class GOSHARED_EXPORT GoFile final : public Core::IDocument
{
public:
    GoFile(const Utils::FileName& fileName);
    ReloadBehavior reloadBehavior(ChangeTrigger, ChangeType) const override;
};

} // Go
