#include "gocodestylepreferencesfactory.h"

#include <texteditor/simplecodestylepreferences.h>
#include <texteditor/texteditor.h>
#include <texteditor/indenter.h>

#include <QLabel>

#include "../goconstants.h"

using namespace Go;

Core::Id CodeStylePreferencesFactory::languageId()
{
    return Constants::SettingsID;
}

QString CodeStylePreferencesFactory::displayName()
{
    return QLatin1String("Go");
}

TextEditor::ICodeStylePreferences* CodeStylePreferencesFactory::createCodeStyle() const
{
    return new TextEditor::SimpleCodeStylePreferences();
}

QWidget* CodeStylePreferencesFactory::createEditor(TextEditor::ICodeStylePreferences*, QWidget* parent) const
{
    return new QLabel(tr("There's no configuration widget yet, sorry."), parent);
}

TextEditor::Indenter* CodeStylePreferencesFactory::createIndenter() const
{
    return new TextEditor::Indenter();
}

TextEditor::ISnippetProvider* CodeStylePreferencesFactory::snippetProvider() const
{
    return nullptr;
}

QString CodeStylePreferencesFactory::previewText() const
{
    return QLatin1String(
                "// Package doc.\n"
                "package main\n"
                "\n"
                "import (\n"
                "   \"flag\"\n"
                ")\n"
                "\n"
                "const kConstant = 5\n"
                "\n"
                "var aVar = 'c'\n"
                "\n"
                "func main() {\n"
                "   flag.Parse()\n"
                "   aVar := 1 + 2\n"
                "\n"
                "   if aVar == 3 {\n"
                "       println(aVar)\n"
                "   }\n"
                "}\n");
}
