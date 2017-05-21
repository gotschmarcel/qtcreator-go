#pragma once

#include <texteditor/texteditor.h>
#include <utils/uncommentselection.h>

#include "../go_global.h"

namespace Go {
namespace Internal {

class GOSHARED_EXPORT EditorWidget final : public TextEditor::TextEditorWidget
{
    Q_OBJECT

public:
    EditorWidget();
};

} // namespace Internal
} // namespace Go
