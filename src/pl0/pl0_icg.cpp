#include "pl0_icg.h"

#include <sstream>

// ============================================
// TODO: 成员3 实现以下函数
// ============================================

Token &PL0ICG::peek() {
    // TODO: 返回当前 token（不移动指针）
    // 注意：需要检查 ip 是否越界
    static Token dummy; // 占位
    return dummy;
}

Token &PL0ICG::advance() {
    // TODO: 返回当前 token 并移动指针到下一个
    static Token dummy; // 占位
    return dummy;
}

bool PL0ICG::accept(const std::string &type) {
    // TODO: 如果当前 token 类型匹配，接受并移动指针
    return false; // 占位
}

bool PL0ICG::expect(const std::string &type, std::string &errorMsg) {
    // TODO: 期望当前 token 类型匹配
    // 不匹配时设置错误信息（包含行号列号）并返回 false
    return false; // 占位
}

std::string PL0ICG::newTemp() {
    // TODO: 生成新的临时变量名（如 "t1", "t2", ...）
    return ""; // 占位
}

std::string PL0ICG::newLabel() {
    // TODO: 生成新的标签名（如 "L1", "L2", ...）
    return ""; // 占位
}

bool PL0ICG::generate(const std::string &source,
                      std::vector<std::string> &tac,
                      std::string &errorMsg) {
    // TODO: 实现三地址码生成入口
    // 1. 重置状态（清空符号表、代码、计数器）
    // 2. 调用 lexer.tokenize() 进行词法分析
    // 3. 设置 ip = 0
    // 4. 调用 parseProgram() 进行语法分析
    // 5. 将生成的 code 复制到 tac
    // 6. 返回是否成功
    return false; // 占位
}

bool PL0ICG::parseProgram(std::string &errorMsg) {
    // TODO: 解析程序：program ::= block "."
    // 1. 调用 parseBlock()
    // 2. 期望 "."
    return false; // 占位
}

bool PL0ICG::parseBlock(std::string &errorMsg) {
    // TODO: 解析块：block ::= [constDecl] [varDecl] "begin" statementList "end"
    // 1. 可选解析 const 声明（识别 "const" 关键字）
    //    - 解析 constItem（id "=" num）
    //    - 解析 constMore（"," constItem constMore | epsilon）
    //    - 期望 ";"
    //    - 将常量加入符号表（isConst=true, constValue=值）
    // 2. 可选解析 var 声明（识别 "var" 关键字）
    //    - 解析 id
    //    - 解析 varMore（"," id varMore | epsilon）
    //    - 期望 ";"
    //    - 将变量加入符号表（isConst=false）
    // 3. 期望 "begin"
    // 4. 解析 statementList
    // 5. 期望 "end"
    return false; // 占位
}

bool PL0ICG::parseStatement(std::string &errorMsg) {
    // TODO: 解析语句
    // statement ::= id ":=" expression
    //            | "begin" statementList "end"
    //            | "if" condition "then" statement
    //            | "while" condition "do" statement
    // 
    // 1. 如果当前 token 是 id：
    //    - 检查符号表中是否存在（不存在则报错）
    //    - 期望 ":="
    //    - 调用 parseExpression() 得到结果临时变量
    //    - 生成赋值代码：id " = " temp
    // 2. 如果当前 token 是 "begin"：
    //    - 解析 statementList（递归调用 parseStatement）
    // 3. 如果当前 token 是 "if"：
    //    - 调用 parseCondition() 得到条件结果临时变量
    //    - 期望 "then"
    //    - 生成条件跳转：ifFalse condTemp goto labelElse
    //    - 解析 then 分支的 statement
    //    - 生成 labelElse 标签
    // 4. 如果当前 token 是 "while"：
    //    - 生成 labelBegin 标签
    //    - 调用 parseCondition() 得到条件结果临时变量
    //    - 生成条件跳转：ifFalse condTemp goto labelEnd
    //    - 期望 "do"
    //    - 解析循环体的 statement
    //    - 生成跳转：goto labelBegin
    //    - 生成 labelEnd 标签
    return false; // 占位
}

bool PL0ICG::parseCondition(std::string &resultTemp, std::string &errorMsg) {
    // TODO: 解析条件表达式
    // condition ::= "odd" expression
    //            | expression relop expression
    // 
    // 1. 如果当前 token 是 "odd"：
    //    - 解析 expression
    //    - 生成：temp = exprTemp % 2; temp2 = temp != 0
    //    - resultTemp = temp2
    // 2. 否则：
    //    - 解析左表达式
    //    - 识别关系运算符（=, #, <, <=, >, >=）
    //    - 解析右表达式
    //    - 生成：temp = leftTemp relop rightTemp
    //    - resultTemp = temp
    return false; // 占位
}

bool PL0ICG::parseExpression(std::string &resultTemp, std::string &errorMsg) {
    // TODO: 解析表达式
    // expression ::= ["+"|"-"] term { ("+"|"-") term }
    // 
    // 1. 可选解析符号（"+" 或 "-"）
    // 2. 解析第一个 term
    // 3. 如果有符号且是 "-"，生成：temp = 0 - termTemp; termTemp = temp
    // 4. 循环处理后续的 "+" term 或 "-" term：
    //    - 解析下一个 term
    //    - 生成：newTemp = resultTemp op termTemp
    //    - resultTemp = newTemp
    return false; // 占位
}

bool PL0ICG::parseTerm(std::string &resultTemp, std::string &errorMsg) {
    // TODO: 解析项
    // term ::= factor { ("*"|"/") factor }
    // 
    // 1. 解析第一个 factor
    // 2. 循环处理后续的 "*" factor 或 "/" factor：
    //    - 解析下一个 factor
    //    - 生成：newTemp = resultTemp op factorTemp
    //    - resultTemp = newTemp
    return false; // 占位
}

bool PL0ICG::parseFactor(std::string &resultTemp, std::string &errorMsg) {
    // TODO: 解析因子
    // factor ::= id | num | "(" expression ")"
    // 
    // 1. 如果当前 token 是 id：
    //    - 检查符号表（不存在则报错）
    //    - resultTemp = id 名称
    // 2. 如果当前 token 是 num：
    //    - 生成临时变量：temp = num值
    //    - resultTemp = temp
    // 3. 如果当前 token 是 "("：
    //    - 解析 expression
    //    - 期望 ")"
    //    - resultTemp = expression 的结果临时变量
    return false; // 占位
}

