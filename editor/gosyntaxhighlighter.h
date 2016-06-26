#pragma once

#include <texteditor/textdocumentlayout.h>
#include <texteditor/syntaxhighlighter.h>

#include <QMap>

namespace Go {

class Token;

class SyntaxHighlighter final : public TextEditor::SyntaxHighlighter
{
public:
    SyntaxHighlighter(QTextDocument* parent = 0);

protected:
    void highlightBlock(const QString& text) override;

private:
    int highlightLine(const QString& text, int state);
    QTextCharFormat formatForToken(const Token& token, const QString& val);

    static QMap<int, int> kCategories;
};

} // Go
