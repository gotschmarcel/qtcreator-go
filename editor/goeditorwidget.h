#pragma once

#include <texteditor/texteditor.h>
#include <utils/uncommentselection.h>

#include "../go_global.h"

namespace Go {
namespace Internal {

class GoEditorWidget : public TextEditor::TextEditorWidget
{
    Q_OBJECT

public:
    GoEditorWidget();
};

} // namespace Internal
} // namespace Go
