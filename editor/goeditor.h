#pragma once

#include <texteditor/texteditor.h>

#include "../go_global.h"

namespace Go {

class GOSHARED_EXPORT Editor final : public TextEditor::BaseTextEditor
{
    Q_OBJECT

public:
    Editor();
};

} // Go
