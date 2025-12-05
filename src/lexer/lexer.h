#pragma once

#include <string>
#include <vector>
#include <map>
#include <set>

struct Token {
    std::string type;    // 词法单元类型（如 "+", "id", "num", "if"）
    std::string lexeme;  // 原始文本
    int line = 1;
    int column = 1;
};

class Lexer {
public:
    Lexer() = default;

    // TODO: 成员1 需要实现
    // 根据文法中的终结符集合配置词法分析器（第 1 部分）
    // 要求：
    // - 识别关键字（纯字母的终结符，如 "if", "while"）
    // - 识别运算符/分隔符（如 "+", "(", ":="）
    // - 如果终结符集合包含 "id"，则启用标识符识别
    // - 如果终结符集合包含 "num"，则启用数字识别
    void configureFromTerminals(const std::set<std::string> &terminals);

    // TODO: 成员1 需要实现
    // 对源程序进行词法分析
    // 要求：
    // - 跳过空白字符（空格、制表符、换行）
    // - 识别标识符（字母或下划线开头，后跟字母、数字、下划线）
    // - 识别数字（纯数字序列）
    // - 识别关键字和运算符（最长匹配）
    // - 记录每个 token 的行号和列号
    // - 在 token 序列末尾添加结束标记 "$"
    // - 遇到无法识别的字符时，返回 false 并设置错误信息
    bool tokenize(const std::string &source,
                  std::vector<Token> &outTokens,
                  std::string &errorMsg);

    // TODO: 成员1 需要实现
    // 创建固定的 PL/0 词法分析器（第 2 部分）
    // 返回一个配置好的 Lexer，包含 PL/0 的关键字和运算符
    static Lexer createPL0Lexer();

private:
    std::set<std::string> keywordTokens;   // 关键字（例如 "if"）
    std::set<std::string> symbolTokens;    // 运算符和分隔符（如 "+", "(", ":="）
    bool hasIdToken = false;
    bool hasNumToken = false;

    // TODO: 成员1 需要实现
    // 判断字符是否可以作为标识符的开头
    bool isIdentifierStart(char c) const;
    
    // TODO: 成员1 需要实现
    // 判断字符是否可以作为标识符的一部分
    bool isIdentifierChar(char c) const;
};

