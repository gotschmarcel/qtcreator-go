#include "goindenter.h"

#include <texteditor/tabsettings.h>

#include <QChar>
#include <QTextBlock>

#include "goscanner.h"

using namespace Go::Internal;

Indenter::Indenter() {}

bool Indenter::isElectricCharacter(const QChar &ch) const
{
    return ch == QLatin1Char('{') || ch == QLatin1Char('(') || ch == QLatin1Char('[');
}

int Indenter::indentFor(const QTextBlock &block, const TextEditor::TabSettings &tabSettings)
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

bool Indenter::isElectricLine(const QString &previousLine) const
{
    if (previousLine.isEmpty()) {
        return false;
    }

    Scanner scanner(previousLine);
    Token lastToken;

    for (Token t = scanner.read(); t.kind != Token::EOF; t = scanner.read()) {
        if (t.kind == Token::COMMENT) {
            continue;
        }

        lastToken = t;
    }

    return isElectricCharacter(scanner.character(lastToken));
}

int Indenter::indentDiff(const QString &previousLine,
                         const TextEditor::TabSettings &tabSettings) const
{
    Scanner scanner(previousLine);
    int parens = 0;
    Token lastToken;

    for (Token t = scanner.read(); t.kind != Token::EOF; t = scanner.read()) {
        switch (t.kind) {
        case Token::COMMENT: continue;

        case Token::LPAREN:
        case Token::LBRACK:
        case Token::LBRACE: ++parens; break;

        case Token::RPAREN:
        case Token::RBRACK:
        case Token::RBRACE: --parens; break;

        default: break;
        }

        lastToken = t;
    }

    if (parens != 0
        && (lastToken.kind == Token::RPAREN || lastToken.kind == Token::RBRACK
            || lastToken.kind == Token::RBRACE)) {
        return -tabSettings.m_indentSize;
    }

    return 0;
}
