#pragma once

#include "../lexer/lexer.h"

#include <string>
#include <vector>
#include <map>

// PL/0 语言的三地址码中间代码生成器

class PL0ICG {
public:
    // DONE: 成员3 已实现
    // PL/0 中间代码生成器构造函数
    // 实现说明：
    // - 接收词法分析后的 token 序列作为输入
    // - 初始化解析位置、计数器和符号表
    explicit PL0ICG(const std::vector<Token>& tokens);

    // DONE: 成员3 已实现
    // PL/0 程序的三地址码生成主入口
    // 实现说明：
    // - 调用递归下降语法分析器解析程序
    // - 生成三地址码序列
    // - 使用异常处理机制报告语法错误
    // - 成功时返回 true 并将三地址码输出到 outTac
    bool generate(std::vector<std::string>& outTac, std::string& error);

private:
    const std::vector<Token>& tokens;
    size_t pos;
    int tempCount;
    int labelCount;
    std::vector<std::string> tac;

    // 符号表：记录常量和变量声明
    struct Symbol {
        std::string type; // "const" or "var"
        std::string value; // 常量的字符串值
    };
    std::map<std::string, Symbol> symbolTable;

    // DONE: 成员3 已实现
    // 查看当前 token（不移动解析位置）
    // 实现说明：返回当前位置的 token，如果越界返回结束标记
    Token peek() const;

    // DONE: 成员3 已实现
    // 读取当前 token 并前进解析位置
    // 实现说明：返回当前位置的 token 并将位置前进一位
    Token next();

    // DONE: 成员3 已实现
    // 期望当前 token 类型匹配，否则抛出异常
    // 实现说明：
    // - 检查当前 token 类型是否与期望匹配
    // - 匹配则消费 token，不匹配则抛出运行时异常
    void expect(const std::string& type, const std::string& errMsg);

    // DONE: 成员3 已实现
    // 生成新的临时变量名（如 t1, t2, ...）
    // 实现说明：使用递增计数器生成唯一的临时变量名称
    std::string newTemp();

    // DONE: 成员3 已实现
    // 生成新的标签名（如 L1, L2, ...）
    // 实现说明：使用递增计数器生成唯一的标签名称
    std::string newLabel();

    // DONE: 成员3 已实现
    // 发出三地址码指令
    // 实现说明：将指令字符串添加到三地址码序列中
    void emit(const std::string& code);

    // DONE: 成员3 已实现
    // 解析程序：program ::= block "."
    // 实现说明：
    // - 调用 parseBlock() 解析程序块
    // - 期望程序结束符 "."
    void parseProgram();

    // DONE: 成员3 已实现
    // 解析块：block ::= [const...] [var...] statement
    // 实现说明：
    // - 可选解析常量声明序列
    // - 可选解析变量声明序列
    // - 解析语句
    void parseBlock();

    // DONE: 成员3 已实现
    // 解析语句：statement ::= id := expr | begin...end | if...then | while...do
    // 实现说明：
    // - 支持赋值语句、复合语句、条件语句和循环语句
    // - 赋值语句检查目标是否为常量
    void parseStatement();

    // DONE: 成员3 已实现
    // 解析表达式，返回结果临时变量名
    // expression ::= ["+"|"-"] term { ("+"|"-") term }
    // 实现说明：
    // - 支持加减运算
    // - 处理可选的正负号
    // - 生成相应的三地址码
    std::string parseExpression();

    // DONE: 成员3 已实现
    // 解析项，返回结果临时变量名
    // term ::= factor { ("*"|"/") factor }
    // 实现说明：
    // - 支持乘除运算
    // - 生成相应的三地址码
    std::string parseTerm();

    // DONE: 成员3 已实现
    // 解析因子，返回结果临时变量名或标识符名
    // factor ::= id | num | "(" expression ")"
    // 实现说明：
    // - 支持标识符、数字常量和括号表达式
    // - 检查标识符是否已声明
    // - 为数字常量生成临时变量赋值
    std::string parseFactor();

    // DONE: 成员3 已实现
    // 解析条件表达式，返回结果临时变量名
    // condition ::= "odd" expression | expression relop expression
    // 实现说明：
    // - 支持 odd 运算和关系运算
    // - 生成相应的三地址码
    std::string parseCondition();
};
