#pragma once

#include <texteditor/texteditor.h>

#include "../go_global.h"

namespace Go {
namespace Internal {

class GOSHARED_EXPORT Editor final : public TextEditor::BaseTextEditor
{
    Q_OBJECT

public:
    Editor();
};

} // Internal
} // Go
