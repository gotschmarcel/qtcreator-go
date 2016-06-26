#pragma once

#include <texteditor/icodestylepreferencesfactory.h>

#include "../go_global.h"

namespace Go {

class GOSHARED_EXPORT CodeStylePreferencesFactory final : public TextEditor::ICodeStylePreferencesFactory
{
public:
    Core::Id languageId() override;
    QString displayName() override;
    TextEditor::ICodeStylePreferences* createCodeStyle() const override;
    QWidget* createEditor(TextEditor::ICodeStylePreferences *, QWidget *parent) const override;
    TextEditor::Indenter* createIndenter() const override;
    TextEditor::ISnippetProvider* snippetProvider() const override;
    QString previewText() const override;
};

} // Go
