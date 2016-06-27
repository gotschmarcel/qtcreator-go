#pragma once

#include <texteditor/indenter.h>

#include "../go_global.h"

namespace Go {

class GOSHARED_EXPORT Indenter final : public TextEditor::Indenter
{
public:
    Indenter();

    bool isElectricCharacter(const QChar& ch) const override;
    int indentFor(const QTextBlock& block, const TextEditor::TabSettings& tabSettings) override;

protected:
    bool isElectricLine(const QString& previousLine) const;
    int indentDiff(const QString& previousLine, const TextEditor::TabSettings& tabSettings) const;
};

} // Go
