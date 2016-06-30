#pragma once

#include <texteditor/texteditor.h>

#include "../go_global.h"

namespace Go {
namespace Internal {

class GOSHARED_EXPORT EditorFactory final : public TextEditor::TextEditorFactory {
    Q_OBJECT

public:
    EditorFactory();
};

} // Internal
} // Go
