#pragma once

#include <texteditor/texteditor.h>

#include "../go_global.h"

namespace Go {
namespace Internal {

class Editor : public TextEditor::BaseTextEditor
{
    Q_OBJECT

public:
    Editor();
};

} // namespace Internal
} // namespace Go
