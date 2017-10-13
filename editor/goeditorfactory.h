#pragma once

#include <texteditor/texteditor.h>

#include "../go_global.h"

namespace Go {
namespace Internal {

class GoEditorFactory : public TextEditor::TextEditorFactory
{
    Q_OBJECT

public:
    GoEditorFactory();
};

} // namespace Internal
} // namespace Go
