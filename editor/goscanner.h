#pragma once

#include <QtGlobal>

#include "gosourcecodestream.h"

// Undef EOF macro
#undef EOF

namespace Go {
namespace Internal {

class Token {
public:
    enum Kind {
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

    Token(Kind kind = EOF, int pos = 0, int len = 0) : kind(kind), pos(pos), len(len) {}

    Kind kind;
    int pos;
    int len;
};

class Scanner {
    Q_DISABLE_COPY(Scanner)

public:
    enum State { Default, MultiLineComment, MultiLineString };

    Scanner(const QString& text);

    Token read();

    int line() const;
    int column(const Token& t) const;

    int indent() const;

    void setState(int state);
    int state() const;

    QString value(const Token& t) const;
    QChar character(const Token& t) const;

private:
    Token readIdentifier();

    Token readMultiLineComment();
    Token readLineComment();

    Token readOperator();

    Token readMultiLineStringLiteral();
    Token readStringLiteral();
    Token readCharLiteral();
    Token readNumberLiteral();

    Token readDefaultState();

    void clearState();

    SourceCodeStream _source;
    int _line;
    int _indent;
    int _state;
};

} // Internal
} // Go
