#pragma once

#include <string>

// 词法单元类型定义，可扩展，根据需要添加
enum class TokenType {
    Identifier,
    Number,
    KeywordIf,
    KeywordThen,
    KeywordBegin,
    KeywordEnd,
    Assign,     // :=
    Plus,       // +
    Minus,      // -
    Semicolon,  // ;
    Greater,    // >
    LParen,     // (
    RParen,     // )
    EndOfFile,
    Unknown
};

struct Token {
    TokenType type;
    std::string lexeme;
    int line;

    Token() : type(TokenType::Unknown), line(0) {}
    Token(TokenType t, const std::string &lex, int ln)
        : type(t), lexeme(lex), line(ln) {}
};

// TODO: 实现一个把 TokenType 转为字符串的函数，便于调试打印
std::string tokenTypeToString(TokenType t);



