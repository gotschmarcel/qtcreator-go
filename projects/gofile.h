#pragma once

#include <coreplugin/idocument.h>

namespace Utils {
class FileName;
}

namespace Go {

class GoFile final : public Core::IDocument
{
public:
    GoFile(const Utils::FileName& fileName);
    ReloadBehavior reloadBehavior(ChangeTrigger, ChangeType) const override;
};

} // Go
