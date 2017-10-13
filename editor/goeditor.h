#pragma once

#include <texteditor/texteditor.h>

#include "../go_global.h"

namespace Go {
namespace Internal {

class GoEditor : public TextEditor::BaseTextEditor
{
    Q_OBJECT

public:
    GoEditor();
};

} // namespace Internal
} // namespace Go
