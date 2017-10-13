#pragma once

#include <QtGlobal>

#include "sourcecodestream.h"

// Undef EOF macro
#undef EOF

namespace Go {
namespace Internal {

class GoToken
{
public:
    enum Kind
    {
        EOF = 0,
        ILLEGAL,
        COMMENT,

        // Identifiers and basic type literals
        // (these tokens stand for classes of literals)
        IDENT,  // main
        INT,    // 12345
        FLOAT,  // 123.45
        IMAG,   // 123.45i
        CHAR,   // 'a'
        STRING, // "abc"

        // Operators and delimiters
        ADD, // +
        SUB, // -
        MUL, // *
        QUO, // /
        REM, // %

        AND,     // &
        OR,      // |
        XOR,     // ^
        SHL,     // <<
        SHR,     // >>
        AND_NOT, // &^

        ADD_ASSIGN, // +=
        SUB_ASSIGN, // -=
        MUL_ASSIGN, // *=
        QUO_ASSIGN, // /=
        REM_ASSIGN, // %=

        AND_ASSIGN,     // &=
        OR_ASSIGN,      // |=
        XOR_ASSIGN,     // ^=
        SHL_ASSIGN,     // <<=
        SHR_ASSIGN,     // >>=
        AND_NOT_ASSIGN, // &^=

        LAND,  // &&
        LOR,   // ||
        ARROW, // <-
        INC,   // ++
        DEC,   // --

        EQL,    // ==
        LSS,    // <
        GTR,    // >
        ASSIGN, // =
        NOT,    // !

        NEQ,      // !=
        LEQ,      // <=
        GEQ,      // >=
        DEFINE,   // :=
        ELLIPSIS, // ...

        LPAREN, // (
        LBRACK, // [
        LBRACE, // {
        COMMA,  // ,
        PERIOD, // .

        RPAREN,    // )
        RBRACK,    // ]
        RBRACE,    // }
        SEMICOLON, // ;
        COLON,     // :

        // Keywords
        BREAK,
        CASE,
        CHAN,
        CONST,
        CONTINUE,

        DEFAULT,
        DEFER,
        ELSE,
        FALLTHROUGH,
        FOR,

        FUNC,
        GO,
        GOTO,
        IF,
        IMPORT,

        INTERFACE,
        MAP,
        PACKAGE,
        RANGE,
        RETURN,

        SELECT,
        STRUCT,
        SWITCH,
        TYPE,
        VAR,

        NUM_TOKENS = VAR
    };

    GoToken(Kind kind = EOF, int pos = 0, int len = 0)
        : kind(kind)
        , pos(pos)
        , len(len)
    {
    }

    Kind kind;
    int pos;
    int len;
};

class GoScanner
{
    Q_DISABLE_COPY(GoScanner)

public:
    enum State
    {
        Default,
        MultiLineComment,
        MultiLineString
    };

    GoScanner(const QString &text);

    GoToken read();

    int line() const;
    int column(const GoToken &t) const;

    int indent() const;

    void setState(int state);
    int state() const;

    QString value(const GoToken &t) const;
    QChar character(const GoToken &t) const;

private:
    GoToken readIdentifier();

    GoToken readMultiLineComment();
    GoToken readLineComment();

    GoToken readOperator();

    GoToken readMultiLineStringLiteral();
    GoToken readStringLiteral();
    GoToken readCharLiteral();
    GoToken readNumberLiteral();

    GoToken readDefaultState();

    void clearState();

    SourceCodeStream _source;
    int _line;
    int _indent;
    int _state;
};

} // namespace Internal
} // namespace Go
