#pragma once

#include <texteditor/texteditor.h>
#include <utils/uncommentselection.h>

#include "../go_global.h"

namespace Go {

class GOSHARED_EXPORT EditorWidget final : public TextEditor::TextEditorWidget
{
    Q_OBJECT

public:
    EditorWidget();
};

} // Go
