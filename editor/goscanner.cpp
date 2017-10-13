#include "goscanner.h"

#include <QMap>

namespace Go {
namespace Internal {


static const QMap<QString, GoToken::Kind> GoKeywords{
    {QLatin1String("break"), GoToken::BREAK},
    {QLatin1String("case"), GoToken::CASE},
    {QLatin1String("chan"), GoToken::CHAN},
    {QLatin1String("const"), GoToken::CONST},
    {QLatin1String("continue"), GoToken::CONTINUE},
    {QLatin1String("default"), GoToken::DEFAULT},
    {QLatin1String("defer"), GoToken::DEFER},
    {QLatin1String("else"), GoToken::ELSE},
    {QLatin1String("fallthrough"), GoToken::FALLTHROUGH},
    {QLatin1String("for"), GoToken::FOR},
    {QLatin1String("func"), GoToken::FUNC},
    {QLatin1String("go"), GoToken::GO},
    {QLatin1String("goto"), GoToken::GOTO},
    {QLatin1String("if"), GoToken::IF},
    {QLatin1String("import"), GoToken::IMPORT},
    {QLatin1String("interface"), GoToken::INTERFACE},
    {QLatin1String("map"), GoToken::MAP},
    {QLatin1String("package"), GoToken::PACKAGE},
    {QLatin1String("range"), GoToken::RANGE},
    {QLatin1String("return"), GoToken::RETURN},
    {QLatin1String("select"), GoToken::SELECT},
    {QLatin1String("struct"), GoToken::STRUCT},
    {QLatin1String("switch"), GoToken::SWITCH},
    {QLatin1String("type"), GoToken::TYPE},
    {QLatin1String("var"), GoToken::VAR},
};

static const QString GoOperators(QLatin1String("+-*/%&|^<>=!;:.,()[]{}"));

inline static bool isLetter(const QChar &c) { return c.isLetter() || c == QLatin1Char('_'); }

inline static bool isDecimalDigit(const QChar &c)
{
    return c >= QLatin1Char('0') && c <= QLatin1Char('9');
}

inline static bool isOctalDigit(const QChar &c)
{
    return c >= QLatin1Char('0') && c <= QLatin1Char('7');
}

inline static bool isHexDigit(const QChar &c)
{
    return c.isDigit() || (c >= QLatin1Char('a') && c <= QLatin1Char('f'))
           || (c >= QLatin1Char('A') && c <= QLatin1Char('F'));
}

inline static bool isNumberLiteral(const QChar &c1, const QChar &c2)
{
    return c1.isDigit()
           || (c1 == QLatin1Char('.')
               && (c2.isDigit() || c2 == QLatin1Char('e') || c2 == QLatin1Char('E')
                   || c2 == QLatin1Char('i')));
}

inline static bool isHexLiteral(const QChar &c1, const QChar &c2)
{
    return c1 == QLatin1Char('0') && (c2 == QLatin1Char('x') || c2 == QLatin1Char('X'));
}

inline static bool isExponent(const QChar &c)
{
    return c == QLatin1Char('e') || c == QLatin1Char('E');
}

GoScanner::GoScanner(const QString &text)
    : _source(text)
    , _line(0)
    , _indent(0)
    , _state(Default)
{
}

GoToken GoScanner::read()
{
    if (_source.atEnd()) {
        return {GoToken::EOF, _source.pos(), 0};
    }

    switch (_state) {
    case MultiLineString: return readMultiLineStringLiteral();
    case MultiLineComment: return readMultiLineComment();
    default: return readDefaultState();
    }
}

int GoScanner::line() const { return _line; }

int GoScanner::column(const GoToken &t) const { return t.pos; }

int GoScanner::indent() const { return _indent; }

void GoScanner::setState(int state) { _state = state; }

int GoScanner::state() const { return _state; }

QString GoScanner::value(const GoToken &t) const { return _source.value(t.pos, t.len).toString(); }

QChar GoScanner::character(const GoToken &t) const
{
    if (t.len == 0) {
        return QChar();
    }

    return _source.value(t.pos, t.len).at(0);
}

GoToken GoScanner::readIdentifier()
{
    QChar c = _source.peek();

    while (isLetter(c) || c.isDigit()) {
        _source.advance();
        c = _source.peek();
    }

    const auto ident = _source.value().toString();

    if (GoKeywords.contains(ident)) {
        return {GoKeywords.value(ident), _source.anchor(), _source.len()};
    }

    return {GoToken::IDENT, _source.anchor(), _source.len()};
}

GoToken GoScanner::readMultiLineComment()
{
    _state = MultiLineComment;

    QChar c = _source.peek();

    while (c != QLatin1Char('*') || _source.peek(1) != QLatin1Char('/')) {
        _source.advance();

        if (_source.atEnd()) {
            break;
        }

        c = _source.peek();
    }

    if (!_source.atEnd() && c == QLatin1Char('*')) {
        _source.advance(); // Eat *
        _source.advance(); // Eat '/'
        clearState();
    }

    return {GoToken::COMMENT, _source.anchor(), _source.len()};
}

GoToken GoScanner::readLineComment()
{
    QChar c = _source.peek();

    while (c != QLatin1Char('\n')) {
        _source.advance();

        if (_source.atEnd()) {
            break;
        }

        c = _source.peek();
    }

    _source.advance(); // Eat '\n'

    return {GoToken::COMMENT, _source.anchor(), _source.len()};
}

GoToken GoScanner::readOperator()
{
    const QChar c1 = _source.peek();
    _source.advance(); // Eat beginning

    // ++, +=, +
    if (c1 == QLatin1Char('+')) {
        const QChar c2 = _source.peek();
        // ++
        if (c2 == QLatin1Char('+')) {
            _source.advance(); // Eat second '+'
            return {GoToken::INC, _source.anchor(), _source.len()};
        }

        // +=
        if (c2 == QLatin1Char('=')) {
            _source.advance(); // Eat '='
            return {GoToken::ADD_ASSIGN, _source.anchor(), _source.len()};
        }

        // Simple +
        return {GoToken::ADD, _source.anchor(), _source.len()};
    }

    // --, -=, -
    if (c1 == QLatin1Char('-')) {
        const QChar c2 = _source.peek();
        // --
        if (c2 == QLatin1Char('-')) {
            _source.advance(); // Eat second '-'
            return {GoToken::DEC, _source.anchor(), _source.len()};
        }

        // -=
        if (c2 == QLatin1Char('=')) {
            _source.advance(); // Eat '='
            return {GoToken::SUB_ASSIGN, _source.anchor(), _source.len()};
        }

        // Simple -
        return {GoToken::SUB, _source.anchor(), _source.len()};
    }

    // *=, *
    if (c1 == QLatin1Char('*')) {
        // *=
        if (_source.peek() == QLatin1Char('=')) {
            _source.advance(); // Eat '='
            return {GoToken::MUL_ASSIGN, _source.anchor(), _source.len()};
        }

        // Simple *
        return {GoToken::MUL, _source.anchor(), _source.len()};
    }

    // /=, /
    if (c1 == QLatin1Char('/')) {
        // /=
        if (_source.peek() == QLatin1Char('=')) {
            _source.advance(); // Eat '='
            return {GoToken::QUO_ASSIGN, _source.anchor(), _source.len()};
        }

        // Simple /
        return {GoToken::QUO, _source.anchor(), _source.len()};
    }

    // %=, %
    if (c1 == QLatin1Char('%')) {
        // %=
        if (_source.peek() == QLatin1Char('=')) {
            _source.advance(); // Eat '='
            return {GoToken::REM_ASSIGN, _source.anchor(), _source.len()};
        }

        // Simple %
        return {GoToken::REM, _source.anchor(), _source.len()};
    }

    // &&, &=, &^, &^=, &
    if (c1 == QLatin1Char('&')) {
        const QChar c2 = _source.peek();

        // &&
        if (c2 == QLatin1Char('&')) {
            _source.advance(); // Eat second '&'
            return {GoToken::LAND, _source.anchor(), _source.len()};
        }

        // &=
        if (c2 == QLatin1Char('=')) {
            _source.advance(); // Eat '='
            return {GoToken::AND_ASSIGN, _source.anchor(), _source.len()};
        }

        // &^=, &^
        if (c2 == QLatin1Char('^')) {
            _source.advance(); // Eat '^'

            // &^=
            if (_source.peek() == QLatin1Char('=')) {
                _source.advance(); // Eat '='
                return {GoToken::AND_NOT_ASSIGN, _source.anchor(), _source.len()};
            }

            return {GoToken::AND_NOT, _source.anchor(), _source.len()};
        }

        // Simple &
        return {GoToken::AND, _source.anchor(), _source.len()};
    }

    // ||, |=, |
    if (c1 == QLatin1Char('|')) {
        const QChar c2 = _source.peek();

        // ||
        if (c2 == QLatin1Char('|')) {
            _source.advance(); // Eat second '|'
            return {GoToken::LOR, _source.anchor(), _source.len()};
        }

        // |=
        if (c2 == QLatin1Char('=')) {
            _source.advance(); // Eat '='
            return {GoToken::OR_ASSIGN, _source.anchor(), _source.len()};
        }

        // Simple |
        return {GoToken::OR, _source.anchor(), _source.len()};
    }

    // ^=, ^
    if (c1 == QLatin1Char('^')) {
        // ^=
        if (_source.peek() == QLatin1Char('=')) {
            _source.advance(); // Eat '='
            return {GoToken::XOR_ASSIGN, _source.anchor(), _source.len()};
        }

        // Simple ^
        return {GoToken::XOR, _source.anchor(), _source.len()};
    }

    // <<, <-, <=, <<=, <
    if (c1 == QLatin1Char('<')) {
        const QChar c2 = _source.peek();

        // << or <<=
        if (c2 == QLatin1Char('<')) {
            _source.advance(); // Eat second '<'

            // <<=
            if (_source.peek() == QLatin1Char('=')) {
                _source.advance(); // Eat '='
                return {GoToken::SHL_ASSIGN, _source.anchor(), _source.len()};
            }

            // <<
            return {GoToken::SHL, _source.anchor(), _source.len()};
        }

        // <-
        if (c2 == QLatin1Char('-')) {
            _source.advance(); // Eat '-'
            return {GoToken::ARROW, _source.anchor(), _source.len()};
        }

        // <=
        if (c2 == QLatin1Char('=')) {
            _source.advance(); // Eat '='
            return {GoToken::LEQ, _source.anchor(), _source.len()};
        }

        // Simple <
        return {GoToken::LSS, _source.anchor(), _source.len()};
    }

    // >>, >=, >>=, >
    if (c1 == QLatin1Char('>')) {
        const QChar c2 = _source.peek();

        // >> or >>=
        if (c2 == QLatin1Char('>')) {
            _source.advance(); // Eat second '>'

            // >>=
            if (_source.peek() == QLatin1Char('=')) {
                _source.advance(); // Eat '='
                return {GoToken::SHR_ASSIGN, _source.anchor(), _source.len()};
            }

            // >>
            return {GoToken::SHR, _source.anchor(), _source.len()};
        }

        // >=
        if (c2 == QLatin1Char('=')) {
            _source.advance(); // Eat '='
            return {GoToken::GEQ, _source.anchor(), _source.len()};
        }

        // Simple >
        return {GoToken::GTR, _source.anchor(), _source.len()};
    }

    // ==, =
    if (c1 == QLatin1Char('=')) {
        // ==
        if (_source.peek() == QLatin1Char('=')) {
            _source.advance(); // Eat second '='
            return {GoToken::EQL, _source.anchor(), _source.len()};
        }

        // Simple =
        return {GoToken::ASSIGN, _source.anchor(), _source.len()};
    }

    // !, !=
    if (c1 == QLatin1Char('!')) {
        // !=
        if (_source.peek() == QLatin1Char('=')) {
            _source.advance(); // Eat '='
            return {GoToken::NEQ, _source.anchor(), _source.len()};
        }

        // Simple !
        return {GoToken::NOT, _source.anchor(), _source.len()};
    }

    // :, :=
    if (c1 == QLatin1Char(':')) {
        // :=
        if (_source.peek() == QLatin1Char('=')) {
            _source.advance(); // Eat '='
            return {GoToken::DEFINE, _source.anchor(), _source.len()};
        }

        // Simple :
        return {GoToken::COLON, _source.anchor(), _source.len()};
    }

    // ,
    if (c1 == QLatin1Char(',')) {
        return {GoToken::COMMA, _source.anchor(), _source.len()};
    }

    // ;
    if (c1 == QLatin1Char(';')) {
        return {GoToken::SEMICOLON, _source.anchor(), _source.len()};
    }

    // (
    if (c1 == QLatin1Char('(')) {
        return {GoToken::LPAREN, _source.anchor(), _source.len()};
    }

    // )
    if (c1 == QLatin1Char(')')) {
        return {GoToken::RPAREN, _source.anchor(), _source.len()};
    }

    // [
    if (c1 == QLatin1Char('[')) {
        return {GoToken::LBRACK, _source.anchor(), _source.len()};
    }

    // ]
    if (c1 == QLatin1Char(']')) {
        return {GoToken::RBRACK, _source.anchor(), _source.len()};
    }

    // {
    if (c1 == QLatin1Char('{')) {
        return {GoToken::LBRACE, _source.anchor(), _source.len()};
    }

    // }
    if (c1 == QLatin1Char('}')) {
        return {GoToken::RBRACE, _source.anchor(), _source.len()};
    }

    // ., ...
    if (c1 == QLatin1Char('.')) {
        // ...
        if (_source.peek() == QLatin1Char('.') && _source.peek(1) == QLatin1Char('.')) {
            _source.advance(); // Eat '.'
            _source.advance(); // Eat '.'
            return {GoToken::ELLIPSIS, _source.anchor(), _source.len()};
        }

        // Simple .
        return {GoToken::PERIOD, _source.anchor(), _source.len()};
    }

    // Never reached
    Q_ASSERT_X(false, "Go::Internal::GoScanner::readOperator", "Unknown operator character");
    return {}; // Prevent compiler warning.
}

GoToken GoScanner::readMultiLineStringLiteral()
{
    _state = MultiLineString;

    QChar c = _source.peek();

    while (c != QLatin1Char('`')) {
        if (c == QLatin1Char('\\')) {
            _source.advance();
        }

        _source.advance();

        if (_source.atEnd()) {
            break;
        }

        c = _source.peek();
    }

    if (c == QLatin1Char('`')) {
        _source.advance(); // Eat '`'
        clearState();
    }

    return {GoToken::STRING, _source.anchor(), _source.len()};
}

GoToken GoScanner::readStringLiteral()
{
    QChar c = _source.peek();

    while (c != QLatin1Char('"')) {
        if (c == QLatin1Char('\\')) {
            _source.advance();
        }

        _source.advance();

        if (_source.atEnd()) {
            break;
        }

        c = _source.peek();
    }

    _source.advance(); // Eat '"'

    return {GoToken::STRING, _source.anchor(), _source.len()};
}

GoToken GoScanner::readCharLiteral()
{
    QChar c = _source.peek();

    while (c != QLatin1Char('\'')) {
        if (c == QLatin1Char('\\')) {
            _source.advance();
        }

        _source.advance();

        if (_source.atEnd()) {
            break;
        }

        c = _source.peek();
    }

    _source.advance(); // Eat '\''

    return {GoToken::STRING, _source.anchor(), _source.len()};
}

GoToken GoScanner::readNumberLiteral()
{
    enum NumberState
    {
        NS_INT,
        NS_FRACT,
        NS_EXP
    };

    auto state = NS_INT;
    auto intDigitCheck = isDecimalDigit;
    bool maybeOctal = false;
    bool isOctal = false;

    {
        const QChar c = _source.peek();

        if (c == QLatin1Char('.')) {
            _source.advance(); // Eat '.'
            state = NS_FRACT;
        }

        if (c == QLatin1Char('0')) {
            if (isHexLiteral(c, _source.peek(1))) {
                _source.advance(); // Eat '0'
                _source.advance(); // Eat 'x' or 'X'
                intDigitCheck = isHexDigit;
            } else {
                maybeOctal = true;
                isOctal = true;
            }
        }
    }

    for (; !_source.atEnd(); _source.advance()) {
        const QChar c = _source.peek();

        if (state == NS_INT) {
            if (c == QLatin1Char('.')) {
                state = NS_FRACT;
            } else if (isExponent(c)) {
                state = NS_EXP;
            } else if (!intDigitCheck(c)) {
                break;
            } else if (!isOctalDigit(c)) {
                isOctal = false;
            }
        } else if (state == NS_FRACT) {
            if (isExponent(c)) {
                state = NS_EXP;

                const QChar c2 = _source.peek(1);
                const QChar c3 = _source.peek(2);

                if (c2 == QLatin1Char('+') || c2 == QLatin1Char('-')) {
                    _source.advance(); // Eat 'e' or 'E' - '+' or '-' will be eaten by the end of
                                       // the loop

                    if (!c3.isDigit()) {
                        break;
                    }
                } else if (!c2.isDigit()) {
                    break;
                }

            } else if (!isDecimalDigit(c)) {
                break;
            }
        } else if (!isDecimalDigit(c)) {
            break;
        }
    }

    if (_source.peek() == QLatin1Char('i')) {
        _source.advance(); // Eat 'i'
    }

    if (state == NS_INT && maybeOctal && !isOctal) {
        return {GoToken::ILLEGAL, _source.anchor(), _source.len()};
    }

    return {(state == NS_INT ? GoToken::INT : GoToken::FLOAT), _source.anchor(), _source.len()};
}

GoToken GoScanner::readDefaultState()
{
    // Skip any whitespace.
    while (_source.peek().isSpace()) {
        _source.advance();
    }

    _source.setAnchor();
    QChar c = _source.peek();

    // Parse identifiers and keywords.
    if (isLetter(c)) {
        _source.advance(); // Eat first letter
        return readIdentifier();
    }

    // Parse a line comment.
    if (c == QLatin1Char('/') && c == _source.peek(1)) {
        _source.advance(); // Eat '/'
        _source.advance(); // Eat '/'
        return readLineComment();
    }

    // Parse a multi line comment.
    if (c == QLatin1Char('/') && _source.peek(1) == QLatin1Char('*')) {
        _source.advance(); // Eat '/'
        _source.advance(); // Eat '*'
        return readMultiLineComment();
    }

    // Parse string literals.
    if (c == QLatin1Char('"')) {
        _source.advance(); // Eat '"'
        return readStringLiteral();
    }

    // Parse multi line string literals.
    if (c == QLatin1Char('`')) {
        _source.advance(); // Eat '`'
        return readMultiLineStringLiteral();
    }

    // Parse character literals.
    if (c == QLatin1Char('\'')) {
        _source.advance(); // Eat '\''
        return readCharLiteral();
    }

    // Parse number literals.
    if (isNumberLiteral(c, _source.peek(1))) {
        return readNumberLiteral();
    }

    // Parse operators.
    if (GoOperators.contains(c)) {
        return readOperator();
    }

    // Check if EOF.
    if (_source.atEnd()) {
        return {GoToken::EOF, _source.pos(), 0};
    }

    _source.advance();
    return {GoToken::ILLEGAL, _source.anchor(), _source.len()};
}

void GoScanner::clearState() { _state = Default; }

} // Internal
} // Go
