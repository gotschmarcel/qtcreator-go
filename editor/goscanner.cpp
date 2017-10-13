#include "goscanner.h"

#include <QMap>

namespace Go {
namespace Internal {


static const QMap<QString, Token::Kind> GoKeywords{
    {QLatin1String("break"), Token::BREAK},
    {QLatin1String("case"), Token::CASE},
    {QLatin1String("chan"), Token::CHAN},
    {QLatin1String("const"), Token::CONST},
    {QLatin1String("continue"), Token::CONTINUE},
    {QLatin1String("default"), Token::DEFAULT},
    {QLatin1String("defer"), Token::DEFER},
    {QLatin1String("else"), Token::ELSE},
    {QLatin1String("fallthrough"), Token::FALLTHROUGH},
    {QLatin1String("for"), Token::FOR},
    {QLatin1String("func"), Token::FUNC},
    {QLatin1String("go"), Token::GO},
    {QLatin1String("goto"), Token::GOTO},
    {QLatin1String("if"), Token::IF},
    {QLatin1String("import"), Token::IMPORT},
    {QLatin1String("interface"), Token::INTERFACE},
    {QLatin1String("map"), Token::MAP},
    {QLatin1String("package"), Token::PACKAGE},
    {QLatin1String("range"), Token::RANGE},
    {QLatin1String("return"), Token::RETURN},
    {QLatin1String("select"), Token::SELECT},
    {QLatin1String("struct"), Token::STRUCT},
    {QLatin1String("switch"), Token::SWITCH},
    {QLatin1String("type"), Token::TYPE},
    {QLatin1String("var"), Token::VAR},
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

Scanner::Scanner(const QString &text)
    : _source(text)
    , _line(0)
    , _indent(0)
    , _state(Default)
{
}

Token Scanner::read()
{
    if (_source.atEnd()) {
        return {Token::EOF, _source.pos(), 0};
    }

    switch (_state) {
    case MultiLineString: return readMultiLineStringLiteral();
    case MultiLineComment: return readMultiLineComment();
    default: return readDefaultState();
    }
}

int Scanner::line() const { return _line; }

int Scanner::column(const Token &t) const { return t.pos; }

int Scanner::indent() const { return _indent; }

void Scanner::setState(int state) { _state = state; }

int Scanner::state() const { return _state; }

QString Scanner::value(const Token &t) const { return _source.value(t.pos, t.len).toString(); }

QChar Scanner::character(const Token &t) const
{
    if (t.len == 0) {
        return QChar();
    }

    return _source.value(t.pos, t.len).at(0);
}

Token Scanner::readIdentifier()
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

    return {Token::IDENT, _source.anchor(), _source.len()};
}

Token Scanner::readMultiLineComment()
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

    return {Token::COMMENT, _source.anchor(), _source.len()};
}

Token Scanner::readLineComment()
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

    return {Token::COMMENT, _source.anchor(), _source.len()};
}

Token Scanner::readOperator()
{
    const QChar c1 = _source.peek();
    _source.advance(); // Eat beginning

    // ++, +=, +
    if (c1 == QLatin1Char('+')) {
        const QChar c2 = _source.peek();
        // ++
        if (c2 == QLatin1Char('+')) {
            _source.advance(); // Eat second '+'
            return {Token::INC, _source.anchor(), _source.len()};
        }

        // +=
        if (c2 == QLatin1Char('=')) {
            _source.advance(); // Eat '='
            return {Token::ADD_ASSIGN, _source.anchor(), _source.len()};
        }

        // Simple +
        return {Token::ADD, _source.anchor(), _source.len()};
    }

    // --, -=, -
    if (c1 == QLatin1Char('-')) {
        const QChar c2 = _source.peek();
        // --
        if (c2 == QLatin1Char('-')) {
            _source.advance(); // Eat second '-'
            return {Token::DEC, _source.anchor(), _source.len()};
        }

        // -=
        if (c2 == QLatin1Char('=')) {
            _source.advance(); // Eat '='
            return {Token::SUB_ASSIGN, _source.anchor(), _source.len()};
        }

        // Simple -
        return {Token::SUB, _source.anchor(), _source.len()};
    }

    // *=, *
    if (c1 == QLatin1Char('*')) {
        // *=
        if (_source.peek() == QLatin1Char('=')) {
            _source.advance(); // Eat '='
            return {Token::MUL_ASSIGN, _source.anchor(), _source.len()};
        }

        // Simple *
        return {Token::MUL, _source.anchor(), _source.len()};
    }

    // /=, /
    if (c1 == QLatin1Char('/')) {
        // /=
        if (_source.peek() == QLatin1Char('=')) {
            _source.advance(); // Eat '='
            return {Token::QUO_ASSIGN, _source.anchor(), _source.len()};
        }

        // Simple /
        return {Token::QUO, _source.anchor(), _source.len()};
    }

    // %=, %
    if (c1 == QLatin1Char('%')) {
        // %=
        if (_source.peek() == QLatin1Char('=')) {
            _source.advance(); // Eat '='
            return {Token::REM_ASSIGN, _source.anchor(), _source.len()};
        }

        // Simple %
        return {Token::REM, _source.anchor(), _source.len()};
    }

    // &&, &=, &^, &^=, &
    if (c1 == QLatin1Char('&')) {
        const QChar c2 = _source.peek();

        // &&
        if (c2 == QLatin1Char('&')) {
            _source.advance(); // Eat second '&'
            return {Token::LAND, _source.anchor(), _source.len()};
        }

        // &=
        if (c2 == QLatin1Char('=')) {
            _source.advance(); // Eat '='
            return {Token::AND_ASSIGN, _source.anchor(), _source.len()};
        }

        // &^=, &^
        if (c2 == QLatin1Char('^')) {
            _source.advance(); // Eat '^'

            // &^=
            if (_source.peek() == QLatin1Char('=')) {
                _source.advance(); // Eat '='
                return {Token::AND_NOT_ASSIGN, _source.anchor(), _source.len()};
            }

            return {Token::AND_NOT, _source.anchor(), _source.len()};
        }

        // Simple &
        return {Token::AND, _source.anchor(), _source.len()};
    }

    // ||, |=, |
    if (c1 == QLatin1Char('|')) {
        const QChar c2 = _source.peek();

        // ||
        if (c2 == QLatin1Char('|')) {
            _source.advance(); // Eat second '|'
            return {Token::LOR, _source.anchor(), _source.len()};
        }

        // |=
        if (c2 == QLatin1Char('=')) {
            _source.advance(); // Eat '='
            return {Token::OR_ASSIGN, _source.anchor(), _source.len()};
        }

        // Simple |
        return {Token::OR, _source.anchor(), _source.len()};
    }

    // ^=, ^
    if (c1 == QLatin1Char('^')) {
        // ^=
        if (_source.peek() == QLatin1Char('=')) {
            _source.advance(); // Eat '='
            return {Token::XOR_ASSIGN, _source.anchor(), _source.len()};
        }

        // Simple ^
        return {Token::XOR, _source.anchor(), _source.len()};
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
                return {Token::SHL_ASSIGN, _source.anchor(), _source.len()};
            }

            // <<
            return {Token::SHL, _source.anchor(), _source.len()};
        }

        // <-
        if (c2 == QLatin1Char('-')) {
            _source.advance(); // Eat '-'
            return {Token::ARROW, _source.anchor(), _source.len()};
        }

        // <=
        if (c2 == QLatin1Char('=')) {
            _source.advance(); // Eat '='
            return {Token::LEQ, _source.anchor(), _source.len()};
        }

        // Simple <
        return {Token::LSS, _source.anchor(), _source.len()};
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
                return {Token::SHR_ASSIGN, _source.anchor(), _source.len()};
            }

            // >>
            return {Token::SHR, _source.anchor(), _source.len()};
        }

        // >=
        if (c2 == QLatin1Char('=')) {
            _source.advance(); // Eat '='
            return {Token::GEQ, _source.anchor(), _source.len()};
        }

        // Simple >
        return {Token::GTR, _source.anchor(), _source.len()};
    }

    // ==, =
    if (c1 == QLatin1Char('=')) {
        // ==
        if (_source.peek() == QLatin1Char('=')) {
            _source.advance(); // Eat second '='
            return {Token::EQL, _source.anchor(), _source.len()};
        }

        // Simple =
        return {Token::ASSIGN, _source.anchor(), _source.len()};
    }

    // !, !=
    if (c1 == QLatin1Char('!')) {
        // !=
        if (_source.peek() == QLatin1Char('=')) {
            _source.advance(); // Eat '='
            return {Token::NEQ, _source.anchor(), _source.len()};
        }

        // Simple !
        return {Token::NOT, _source.anchor(), _source.len()};
    }

    // :, :=
    if (c1 == QLatin1Char(':')) {
        // :=
        if (_source.peek() == QLatin1Char('=')) {
            _source.advance(); // Eat '='
            return {Token::DEFINE, _source.anchor(), _source.len()};
        }

        // Simple :
        return {Token::COLON, _source.anchor(), _source.len()};
    }

    // ,
    if (c1 == QLatin1Char(',')) {
        return {Token::COMMA, _source.anchor(), _source.len()};
    }

    // ;
    if (c1 == QLatin1Char(';')) {
        return {Token::SEMICOLON, _source.anchor(), _source.len()};
    }

    // (
    if (c1 == QLatin1Char('(')) {
        return {Token::LPAREN, _source.anchor(), _source.len()};
    }

    // )
    if (c1 == QLatin1Char(')')) {
        return {Token::RPAREN, _source.anchor(), _source.len()};
    }

    // [
    if (c1 == QLatin1Char('[')) {
        return {Token::LBRACK, _source.anchor(), _source.len()};
    }

    // ]
    if (c1 == QLatin1Char(']')) {
        return {Token::RBRACK, _source.anchor(), _source.len()};
    }

    // {
    if (c1 == QLatin1Char('{')) {
        return {Token::LBRACE, _source.anchor(), _source.len()};
    }

    // }
    if (c1 == QLatin1Char('}')) {
        return {Token::RBRACE, _source.anchor(), _source.len()};
    }

    // ., ...
    if (c1 == QLatin1Char('.')) {
        // ...
        if (_source.peek() == QLatin1Char('.') && _source.peek(1) == QLatin1Char('.')) {
            _source.advance(); // Eat '.'
            _source.advance(); // Eat '.'
            return {Token::ELLIPSIS, _source.anchor(), _source.len()};
        }

        // Simple .
        return {Token::PERIOD, _source.anchor(), _source.len()};
    }

    // Never reached
    Q_ASSERT_X(false, "Go::Scanner::readOperator", "Unknown operator character");
    return {}; // Prevent compiler warning.
}

Token Scanner::readMultiLineStringLiteral()
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

    return {Token::STRING, _source.anchor(), _source.len()};
}

Token Scanner::readStringLiteral()
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

    return {Token::STRING, _source.anchor(), _source.len()};
}

Token Scanner::readCharLiteral()
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

    return {Token::STRING, _source.anchor(), _source.len()};
}

Token Scanner::readNumberLiteral()
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
        return {Token::ILLEGAL, _source.anchor(), _source.len()};
    }

    return {(state == NS_INT ? Token::INT : Token::FLOAT), _source.anchor(), _source.len()};
}

Token Scanner::readDefaultState()
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
        return {Token::EOF, _source.pos(), 0};
    }

    _source.advance();
    return {Token::ILLEGAL, _source.anchor(), _source.len()};
}

void Scanner::clearState() { _state = Default; }

} // Internal
} // Go
