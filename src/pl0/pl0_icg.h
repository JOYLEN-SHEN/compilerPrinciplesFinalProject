#pragma once

#include "../lexer/lexer.h"

#include <string>
#include <vector>
#include <map>
#include <stdexcept>

//// 简单的三地址码表示：每条指令用一行字符串表示
//
//struct SymbolInfo {
//    bool isConst = false;
//    int constValue = 0;
//};
//
//class PL0ICG {
//public:
//    PL0ICG() = default;
//
//    // TODO: 成员3 需要实现
//    // PL/0 程序的三地址码生成入口
//    // 要求：
//    // 1. 调用词法分析器对源程序进行词法分析
//    // 2. 调用 parseProgram() 进行语法分析并生成三地址码
//    // 3. 将生成的三地址码输出到 tac 参数
//    // 4. 遇到错误返回 false 并设置错误信息
//    bool generate(const std::string &source,
//                  std::vector<std::string> &tac,
//                  std::string &errorMsg);
//
//private:
//    Lexer lexer = Lexer::createPL0Lexer();
//    std::vector<Token> tokens;
//    size_t ip = 0;
//
//    std::map<std::string, SymbolInfo> symTable;  // 符号表
//    std::vector<std::string> code;               // 生成的三地址码
//
//    int tempCounter = 0;  // 临时变量计数器
//    int labelCounter = 0;  // 标签计数器
//
//    // TODO: 成员3 需要实现
//    // 查看当前 token（不移动指针）
//    Token &peek();
//    
//    // TODO: 成员3 需要实现
//    // 读取当前 token 并移动指针
//    Token &advance();
//    
//    // TODO: 成员3 需要实现
//    // 如果当前 token 类型匹配，则接受并移动指针，否则返回 false
//    bool accept(const std::string &type);
//    
//    // TODO: 成员3 需要实现
//    // 期望当前 token 类型匹配，不匹配则返回 false 并设置错误信息
//    bool expect(const std::string &type, std::string &errorMsg);
//
//    // TODO: 成员3 需要实现
//    // 生成新的临时变量名（如 t1, t2, ...）
//    std::string newTemp();
//    
//    // TODO: 成员3 需要实现
//    // 生成新的标签名（如 L1, L2, ...）
//    std::string newLabel();
//
//    // TODO: 成员3 需要实现
//    // 解析程序：program ::= block "."
//    bool parseProgram(std::string &errorMsg);
//    
//    // TODO: 成员3 需要实现
//    // 解析块：block ::= [constDecl] [varDecl] "begin" statementList "end"
//    bool parseBlock(std::string &errorMsg);
//    
//    // TODO: 成员3 需要实现
//    // 解析语句：statement ::= id ":=" expression | "begin" ... "end" | "if" ... | "while" ...
//    bool parseStatement(std::string &errorMsg);
//
//    // TODO: 成员3 需要实现
//    // 解析条件表达式，返回结果临时变量名
//    // condition ::= "odd" expression | expression relop expression
//    bool parseCondition(std::string &resultTemp, std::string &errorMsg);
//    
//    // TODO: 成员3 需要实现
//    // 解析表达式，返回结果临时变量名
//    // expression ::= ["+"|"-"] term { ("+"|"-") term }
//    bool parseExpression(std::string &resultTemp, std::string &errorMsg);
//    
//    // TODO: 成员3 需要实现
//    // 解析项，返回结果临时变量名
//    // term ::= factor { ("*"|"/") factor }
//    bool parseTerm(std::string &resultTemp, std::string &errorMsg);
//    
//    // TODO: 成员3 需要实现
//    // 解析因子，返回结果临时变量名或标识符名
//    // factor ::= id | num | "(" expression ")"
//    bool parseFactor(std::string &resultTemp, std::string &errorMsg);
//};

class PL0ICG {
public:
    explicit PL0ICG(const std::vector<Token>& tokens);

    // 主入口：生成三地址码
    bool generate(std::vector<std::string>& outTac, std::string& error);

private:
    const std::vector<Token>& tokens;
    size_t pos;
    int tempCount;
    int labelCount;
    std::vector<std::string> tac;

    // 符号表：记录常量和变量
    struct Symbol {
        std::string type; // "const" or "var"
        std::string value; // 常量的值
    };
    std::map<std::string, Symbol> symbolTable;

    // 辅助工具
    Token peek() const;
    Token next();
    void expect(const std::string& type, const std::string& errMsg);
    std::string newTemp();
    std::string newLabel();
    void emit(const std::string& code);

    // 递归下降子程序
    void parseProgram();
    void parseBlock();
    void parseStatement();
    std::string parseExpression();
    std::string parseTerm();
    std::string parseFactor();
    std::string parseCondition();
};
