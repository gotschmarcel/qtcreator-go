#include "gosyntaxhighlighter.h"

#include <QVector>

#include "goscanner.h"

using namespace Go;

static const QVector<QString> GoBuiltins = {
    // Functions
    QLatin1String("cap"),
    QLatin1String("len"),
    QLatin1String("make"),
    QLatin1String("new"),
    QLatin1String("close"),
    QLatin1String("append"),
    QLatin1String("delete"),
    QLatin1String("copy"),
    QLatin1String("complex"),
    QLatin1String("real"),
    QLatin1String("imag"),
    QLatin1String("panic"),
    QLatin1String("recover"),

    // Misc.
    QLatin1String("iota"),

    // Types
    QLatin1String("uint8"),
    QLatin1String("uint16"),
    QLatin1String("uint32"),
    QLatin1String("uint64"),

    QLatin1String("int8"),
    QLatin1String("int16"),
    QLatin1String("int32"),
    QLatin1String("int64"),

    QLatin1String("float32"),
    QLatin1String("float64"),

    QLatin1String("complex64"),
    QLatin1String("complex128"),

    QLatin1String("byte"),
    QLatin1String("rune"),

    QLatin1String("int"),
    QLatin1String("uint"),

    QLatin1String("uintptr"),

    QLatin1String("bool")
};

SyntaxHighlighter::SyntaxHighlighter(QTextDocument* parent)
    : TextEditor::SyntaxHighlighter(parent)
{
    static QVector<TextEditor::TextStyle> categories;

    if (categories.isEmpty()) {
        categories << TextEditor::C_TEXT
                   << TextEditor::C_NUMBER
                   << TextEditor::C_COMMENT
                   << TextEditor::C_KEYWORD
                   << TextEditor::C_TYPE
                   << TextEditor::C_STRING
                   << TextEditor::C_FIELD
                   << TextEditor::C_LABEL
                   << TextEditor::C_LOCAL
                   << TextEditor::C_OPERATOR
                   << TextEditor::C_PARENTHESES
                   << TextEditor::C_PRIMITIVE_TYPE;

        int i = 0;
        for (const auto c : categories) {
            kCategories[c] = i++;
        }
    }

    setTextFormatCategories(categories);
}

void SyntaxHighlighter::highlightBlock(const QString& text)
{
    int state = previousBlockState();
    if (state == -1) {
        state = 0;
    }

    setCurrentBlockState(highlightLine(text, state));
}

int SyntaxHighlighter::highlightLine(const QString& text, int state)
{
    Scanner scanner(text);
    scanner.setState(state);

    Token token;
    while ((token = scanner.read()).kind != Token::EOF) {
        setFormat(token.pos, token.len, formatForToken(token, scanner.value(token)));
    }

    return scanner.state();
}

QTextCharFormat SyntaxHighlighter::formatForToken(const Token& token, const QString& val)
{
    using namespace TextEditor;

    TextStyle style;

    switch (token.kind) {
    case Token::BREAK:
    case Token::CASE:
    case Token::CHAN:
    case Token::CONST:
    case Token::CONTINUE:

    case Token::DEFAULT:
    case Token::DEFER:
    case Token::ELSE:
    case Token::FALLTHROUGH:
    case Token::FOR:

    case Token::FUNC:
    case Token::GO:
    case Token::GOTO:
    case Token::IF:
    case Token::IMPORT:

    case Token::INTERFACE:
    case Token::MAP:
    case Token::PACKAGE:
    case Token::RANGE:
    case Token::RETURN:

    case Token::SELECT:
    case Token::STRUCT:
    case Token::SWITCH:
    case Token::TYPE:
    case Token::VAR:        style = C_KEYWORD; break;

    case Token::COMMENT:    style = C_COMMENT; break;

    case Token::INT:
    case Token::FLOAT:
    case Token::IMAG:       style = C_NUMBER; break;

    case Token::CHAR:
    case Token::STRING:     style = C_STRING; break;

    case Token::ADD:
    case Token::SUB:
    case Token::MUL:
    case Token::QUO:
    case Token::REM:
    case Token::AND:
    case Token::OR:
    case Token::XOR:
    case Token::SHL:
    case Token::SHR:
    case Token::AND_NOT:
    case Token::LAND:
    case Token::LOR:
    case Token::ARROW:
    case Token::INC:
    case Token::DEC:
    case Token::EQL:
    case Token::LSS:
    case Token::GTR:
    case Token::ASSIGN:
    case Token::NOT:
    case Token::NEQ:
    case Token::LEQ:
    case Token::GEQ:
    case Token::DEFINE:
    case Token::ADD_ASSIGN:
    case Token::SUB_ASSIGN:
    case Token::MUL_ASSIGN:
    case Token::QUO_ASSIGN:
    case Token::REM_ASSIGN:
    case Token::AND_ASSIGN:
    case Token::OR_ASSIGN:
    case Token::XOR_ASSIGN:
    case Token::SHL_ASSIGN:
    case Token::SHR_ASSIGN:
    case Token::AND_NOT_ASSIGN:
    case Token::ELLIPSIS:   style = C_OPERATOR; break;

    case Token::LPAREN:
    case Token::LBRACK:
    case Token::LBRACE:
    case Token::RPAREN:
    case Token::RBRACK:
    case Token::RBRACE:     style = C_PARENTHESES; break;

    case Token::COMMA:
    case Token::SEMICOLON:
    case Token::COLON:

    case Token::IDENT:
        if (GoBuiltins.contains(val)) {
            style = C_PRIMITIVE_TYPE;
            break;
        }
    default:                style = C_TEXT; break;
    }

    return formatForCategory(kCategories[style]);
}

QMap<int, int> SyntaxHighlighter::kCategories;
