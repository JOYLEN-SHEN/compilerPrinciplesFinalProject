#include "grammar.h"

#include <fstream>
#include <sstream>
#include <iostream>

// ============================================
// TODO: 成员1 实现以下函数
// ============================================

std::string trim(const std::string &s) {
    // TODO: 实现字符串首尾空白字符的去除
    // 提示：可以使用 std::isspace 判断空白字符
    return s; // 占位，需要替换为实际实现
}

std::vector<std::string> splitSymbols(const std::string &rhs) {
    // TODO: 实现产生式右部的符号拆分
    // 要求：
    // 1. 支持带引号的终结符，如 "if", "+", ":=" 等
    // 2. 按空格拆分非引号内的符号
    // 3. 返回符号列表
    // 提示：需要处理引号内的内容作为一个整体
    return {}; // 占位，需要替换为实际实现
}

bool Grammar::loadFromFile(const std::string &path, std::string &errorMsg) {
    // TODO: 实现文法文件加载
    // 要求：
    // 1. 读取文件，解析 %Start 指令确定开始符号
    // 2. 解析产生式（格式：A -> B C | D，支持 | 分隔多个候选式）
    // 3. 识别非终结符（产生式左部）和终结符（不在非终结符集合中的符号）
    // 4. 调用 computeFirstSets(), computeFollowSets(), buildLL1ParseTable()
    // 5. 错误处理：文件不存在、格式错误等
    // 
    // 文法文件格式示例：
    // %Start Expr
    // Expr -> Term ExprTail
    // ExprTail -> "+" Term ExprTail | epsilon
    // Term -> Factor TermTail
    // ...
    
    return false; // 占位，需要替换为实际实现
}

// ============================================
// TODO: 成员2 实现以下函数
// ============================================

void Grammar::computeFirstSets() {
    // TODO: 实现 FIRST 集合计算
    // 算法：
    // 1. 初始化：对每个终结符 t，FIRST(t) = {t}
    // 2. 对每个非终结符，初始化 FIRST(A) = {}
    // 3. 迭代直到不再变化：
    //    - 对产生式 A -> X1 X2 ... Xn：
    //      * 将 FIRST(X1) - {epsilon} 加入 FIRST(A)
    //      * 如果 X1 可推导出 epsilon，继续处理 X2，以此类推
    //      * 如果所有 Xi 都可推导出 epsilon，将 epsilon 加入 FIRST(A)
    // 4. 对 epsilon 产生式 A -> epsilon，将 epsilon 加入 FIRST(A)
}

void Grammar::computeFollowSets() {
    // TODO: 实现 FOLLOW 集合计算
    // 算法：
    // 1. 初始化：FOLLOW(开始符号) = {$}
    // 2. 迭代直到不再变化：
    //    - 对产生式 A -> αBβ：
    //      * 将 FIRST(β) - {epsilon} 加入 FOLLOW(B)
    //      * 如果 beta 可推导出 epsilon，将 FOLLOW(A) 加入 FOLLOW(B)
    //    - 对产生式 A -> αB（B 在末尾）：
    //      * 将 FOLLOW(A) 加入 FOLLOW(B)
}

bool Grammar::buildLL1ParseTable(std::string &errorMsg) {
    // TODO: 实现 LL(1) 预测分析表构造
    // 算法：
    // 1. 对每个产生式 A -> α：
    //    - 计算 FIRST(α)
    //    - 对每个 a ∈ FIRST(α) - {epsilon}，将 A -> α 加入 M[A, a]
    //    - 如果 epsilon ∈ FIRST(α)，对每个 b ∈ FOLLOW(A)，将 A -> α 加入 M[A, b]
    // 2. 检查冲突：如果 M[A, a] 有多个产生式，报错（非 LL(1) 文法）
    // 3. 返回是否成功
    return false; // 占位，需要替换为实际实现
}

void Grammar::debugPrint() const {
    // 可选：实现调试输出，打印文法、FIRST/FOLLOW 集合、分析表等
}

