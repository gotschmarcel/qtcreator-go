#include "goindenter.h"

#include <texteditor/tabsettings.h>

#include <QChar>
#include <QTextBlock>

#include "goscanner.h"

namespace Go {
namespace Internal {

GoIndenter::GoIndenter() {}

bool GoIndenter::isElectricCharacter(const QChar &ch) const
{
    return ch == QLatin1Char('{') || ch == QLatin1Char('(') || ch == QLatin1Char('[');
}

int GoIndenter::indentFor(const QTextBlock &block, const TextEditor::TabSettings &tabSettings)
{
    const auto &previousBlock = block.previous();
    if (!previousBlock.isValid()) {
        return 0;
    }

    const auto &previousLine = previousBlock.text();
    int indentation = tabSettings.indentationColumn(previousLine);

    if (isElectricLine(previousLine)) {
        indentation += tabSettings.m_indentSize;
    } else {
        indentation = qMax<int>(0, indentation + indentDiff(previousLine, tabSettings));
    }

    return indentation;
}

bool GoIndenter::isElectricLine(const QString &previousLine) const
{
    if (previousLine.isEmpty()) {
        return false;
    }

    GoScanner scanner(previousLine);
    GoToken lastToken;

    for (GoToken t = scanner.read(); t.kind != GoToken::EOF; t = scanner.read()) {
        if (t.kind == GoToken::COMMENT) {
            continue;
        }

        lastToken = t;
    }

    return isElectricCharacter(scanner.character(lastToken));
}

int GoIndenter::indentDiff(const QString &previousLine,
                         const TextEditor::TabSettings &tabSettings) const
{
    GoScanner scanner(previousLine);
    int parens = 0;
    GoToken lastToken;

    for (GoToken t = scanner.read(); t.kind != GoToken::EOF; t = scanner.read()) {
        switch (t.kind) {
        case GoToken::COMMENT: continue;

        case GoToken::LPAREN:
        case GoToken::LBRACK:
        case GoToken::LBRACE: ++parens; break;

        case GoToken::RPAREN:
        case GoToken::RBRACK:
        case GoToken::RBRACE: --parens; break;

        default: break;
        }

        lastToken = t;
    }

    if (parens != 0
        && (lastToken.kind == GoToken::RPAREN || lastToken.kind == GoToken::RBRACK
            || lastToken.kind == GoToken::RBRACE)) {
        return -tabSettings.m_indentSize;
    }

    return 0;
}

} // Internal
} // Go
