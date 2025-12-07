#pragma once

#include <string>
#include <vector>
#include <map>
#include <set>

// 文法与 LL(1) 预测分析表的数据结构

struct Production {
    std::string lhs;
    std::vector<std::string> rhs; // 空 rhs 表示 epsilon
};

struct Grammar {
    std::set<std::string> nonterminals;
    std::set<std::string> terminals;
    std::string startSymbol;
    std::vector<Production> productions;

    // 特殊符号
    const std::string EPSILON = "epsilon";
    const std::string END_MARKER = "$";

    // FIRST / FOLLOW 集合
    std::map<std::string, std::set<std::string>> first;
    std::map<std::string, std::set<std::string>> follow;

    // LL(1) 预测分析表: (Nonterminal, Terminal) -> 产生式编号
    std::map<std::string, std::map<std::string, int>> parseTable;

    // DONE: 成员1 已实现
    // 从文件加载文法，解析产生式，识别终结符和非终结符
    // 实现说明：
    // - 支持 %Start 指令指定起始符号
    // - 解析产生式（支持 | 分隔的多个候选式）
    // - 自动识别终结符（带引号的符号或非大写字母开头的符号）
    // - 自动识别非终结符（大写字母开头的符号）
    // - 支持 epsilon 产生式
    // - 自动调用 computeFirstSets()、computeFollowSets() 和 buildLL1ParseTable()
    bool loadFromFile(const std::string &path, std::string &errorMsg);

    // DONE: 成员2 已实现
    // 计算所有符号的 FIRST 集合
    // 实现说明：使用迭代不动点算法计算 FIRST 集合
    // - 终结符的 FIRST 集合是它自己
    // - 非终结符的 FIRST 集合通过产生式迭代计算
    // - 处理 epsilon 产生式和产生式右部符号序列的 FIRST 集合
    void computeFirstSets();
    
    // DONE: 成员2 已实现
    // 计算所有非终结符的 FOLLOW 集合
    // 实现说明：使用迭代不动点算法计算 FOLLOW 集合
    // - 起始符号的 FOLLOW 集合包含 $
    // - 对于产生式 A -> αBβ，将 FIRST(β) - {epsilon} 添加到 FOLLOW(B)
    // - 如果 β 可以推导出 epsilon，将 FOLLOW(A) 添加到 FOLLOW(B)
    void computeFollowSets();
    
    // DONE: 成员2 已实现
    // 根据 FIRST/FOLLOW 集合构造 LL(1) 预测分析表
    // 实现说明：
    // - 对于产生式 A -> α，将产生式添加到 M[A, a]，其中 a ∈ FIRST(α)
    // - 如果 epsilon ∈ FIRST(α)，将产生式添加到 M[A, b]，其中 b ∈ FOLLOW(A)
    // - 检测 LL(1) 冲突（同一表项有多个产生式）
    bool buildLL1ParseTable(std::string &errorMsg);

    // 调试输出（可选实现）
    void debugPrint() const;
};

// DONE: 成员1 已实现
// 去掉字符串首尾空白字符
// 实现说明：去除字符串首尾的空白字符（空格、制表符、换行等）
std::string trim(const std::string &s);

// DONE: 成员1 已实现
// 按空格拆分产生式右部，支持带引号的终结符（如 "if", "+"）
// 实现说明：
// - 支持带引号的终结符（引号内的内容作为一个整体）
// - 使用空格分隔符号
// - 自动去除引号，保留引号内的内容
std::vector<std::string> splitSymbols(const std::string &rhs);

