#include "lexer.h"

#include <cctype>
#include <sstream>

// ============================================
// TODO: 成员1 实现以下函数
// ============================================

void Lexer::configureFromTerminals(const std::set<std::string> &terminals) {
    // TODO: 根据终结符集合配置词法分析器
    // 要求：
    // 1. 遍历终结符集合
    // 2. 如果终结符是 "id"，设置 hasIdToken = true
    // 3. 如果终结符是 "num"，设置 hasNumToken = true
    // 4. 如果终结符是纯字母（如 "if", "while"），加入 keywordTokens
    // 5. 其他终结符（如 "+", "(", ":="）加入 symbolTokens
}

Lexer Lexer::createPL0Lexer() {
    // TODO: 创建 PL/0 词法分析器
    // 要求：
    // 1. 设置关键字：const, var, begin, end, if, then, while, do, odd
    // 2. 设置运算符/分隔符：:=, <=, >=, #, +, -, *, /, (, ), =, <, >, ,, ., ;
    // 3. 启用 id 和 num 识别
    // 4. 返回配置好的 Lexer 对象
    Lexer lx;
    // TODO: 实现配置
    return lx;
}

bool Lexer::isIdentifierStart(char c) const {
    // TODO: 判断字符是否可以作为标识符开头
    // 要求：字母或下划线返回 true
    return false; // 占位
}

bool Lexer::isIdentifierChar(char c) const {
    // TODO: 判断字符是否可以作为标识符的一部分
    // 要求：字母、数字或下划线返回 true
    return false; // 占位
}

bool Lexer::tokenize(const std::string &source,
                     std::vector<Token> &outTokens,
                     std::string &errorMsg) {
    // TODO: 实现词法分析主函数
    // 算法：
    // 1. 初始化：行号=1，列号=1，清空输出 token 列表
    // 2. 遍历源程序字符串：
    //    - 跳过空白字符（更新行号/列号）
    //    - 识别标识符/关键字：
    //      * 如果以字母/下划线开头，读取完整标识符
    //      * 检查是否为关键字，是则 type=关键字，否则 type="id"
    //    - 识别数字：
    //      * 如果以数字开头，读取完整数字序列
    //      * type="num"
    //    - 识别运算符/分隔符：
    //      * 尝试匹配 symbolTokens 中最长的符号（最长匹配）
    //      * type=匹配到的符号
    //    - 无法识别：返回 false，设置错误信息（包含行号列号）
    // 3. 在 token 列表末尾添加结束标记 "$"
    // 4. 返回 true
    
    outTokens.clear();
    // TODO: 实现词法分析逻辑
    return false; // 占位
}

