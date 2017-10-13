#include "gosyntaxhighlighter.h"

#include <QVector>

#include "goscanner.h"

namespace Go {
namespace Internal {

static const QVector<QString> GoBuiltins = {
    // Functions
    QLatin1String("cap"), QLatin1String("len"), QLatin1String("make"), QLatin1String("new"),
    QLatin1String("close"), QLatin1String("append"), QLatin1String("delete"), QLatin1String("copy"),
    QLatin1String("complex"), QLatin1String("real"), QLatin1String("imag"), QLatin1String("panic"),
    QLatin1String("recover"),

    // Misc.
    QLatin1String("iota"),

    // Types
    QLatin1String("uint8"), QLatin1String("uint16"), QLatin1String("uint32"),
    QLatin1String("uint64"),

    QLatin1String("int8"), QLatin1String("int16"), QLatin1String("int32"), QLatin1String("int64"),

    QLatin1String("float32"), QLatin1String("float64"),

    QLatin1String("complex64"), QLatin1String("complex128"),

    QLatin1String("byte"), QLatin1String("rune"),

    QLatin1String("int"), QLatin1String("uint"),

    QLatin1String("uintptr"),

    QLatin1String("bool")};

GoSyntaxHighlighter::GoSyntaxHighlighter(QTextDocument *parent)
    : TextEditor::SyntaxHighlighter(parent)
{
    setDefaultTextFormatCategories();
}

void GoSyntaxHighlighter::highlightBlock(const QString &text)
{
    int state = previousBlockState();
    if (state == -1) {
        state = 0;
    }

    setCurrentBlockState(highlightLine(text, state));
}

int GoSyntaxHighlighter::highlightLine(const QString &text, int state)
{
    GoScanner scanner(text);
    scanner.setState(state);

    GoToken token;
    while ((token = scanner.read()).kind != GoToken::EOF) {
        setFormat(token.pos, token.len, formatForToken(token, scanner.value(token)));
    }

    return scanner.state();
}

QTextCharFormat GoSyntaxHighlighter::formatForToken(const GoToken &token, const QString &val)
{
    using namespace TextEditor;

    TextStyle style;

    switch (token.kind) {
    case GoToken::BREAK:
    case GoToken::CASE:
    case GoToken::CHAN:
    case GoToken::CONST:
    case GoToken::CONTINUE:

    case GoToken::DEFAULT:
    case GoToken::DEFER:
    case GoToken::ELSE:
    case GoToken::FALLTHROUGH:
    case GoToken::FOR:

    case GoToken::FUNC:
    case GoToken::GO:
    case GoToken::GOTO:
    case GoToken::IF:
    case GoToken::IMPORT:

    case GoToken::INTERFACE:
    case GoToken::MAP:
    case GoToken::PACKAGE:
    case GoToken::RANGE:
    case GoToken::RETURN:

    case GoToken::SELECT:
    case GoToken::STRUCT:
    case GoToken::SWITCH:
    case GoToken::TYPE:
    case GoToken::VAR: style = C_KEYWORD; break;

    case GoToken::COMMENT: style = C_COMMENT; break;

    case GoToken::INT:
    case GoToken::FLOAT:
    case GoToken::IMAG: style = C_NUMBER; break;

    case GoToken::CHAR:
    case GoToken::STRING: style = C_STRING; break;

    case GoToken::ADD:
    case GoToken::SUB:
    case GoToken::MUL:
    case GoToken::QUO:
    case GoToken::REM:
    case GoToken::AND:
    case GoToken::OR:
    case GoToken::XOR:
    case GoToken::SHL:
    case GoToken::SHR:
    case GoToken::AND_NOT:
    case GoToken::LAND:
    case GoToken::LOR:
    case GoToken::ARROW:
    case GoToken::INC:
    case GoToken::DEC:
    case GoToken::EQL:
    case GoToken::LSS:
    case GoToken::GTR:
    case GoToken::ASSIGN:
    case GoToken::NOT:
    case GoToken::NEQ:
    case GoToken::LEQ:
    case GoToken::GEQ:
    case GoToken::DEFINE:
    case GoToken::LPAREN:
    case GoToken::LBRACK:
    case GoToken::LBRACE:
    case GoToken::RPAREN:
    case GoToken::RBRACK:
    case GoToken::RBRACE:
    case GoToken::ADD_ASSIGN:
    case GoToken::SUB_ASSIGN:
    case GoToken::MUL_ASSIGN:
    case GoToken::QUO_ASSIGN:
    case GoToken::REM_ASSIGN:
    case GoToken::AND_ASSIGN:
    case GoToken::OR_ASSIGN:
    case GoToken::XOR_ASSIGN:
    case GoToken::SHL_ASSIGN:
    case GoToken::SHR_ASSIGN:
    case GoToken::AND_NOT_ASSIGN:
    case GoToken::ELLIPSIS: style = C_OPERATOR; break;

    case GoToken::ILLEGAL: style = C_ERROR; break;

    case GoToken::COMMA:
    case GoToken::SEMICOLON:
    case GoToken::COLON:

    case GoToken::IDENT:
        if (GoBuiltins.contains(val)) {
            style = C_PRIMITIVE_TYPE;
            break;
        }

    default: style = C_TEXT; break;
    }

    return formatForCategory(style);
}

} // Internal
} // Go
