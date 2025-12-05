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

    // TODO: 成员1 需要实现
    // 从文件加载文法，解析产生式，识别终结符和非终结符
    bool loadFromFile(const std::string &path, std::string &errorMsg);

    // TODO: 成员2 需要实现
    // 计算所有符号的 FIRST 集合
    void computeFirstSets();
    
    // TODO: 成员2 需要实现
    // 计算所有非终结符的 FOLLOW 集合
    void computeFollowSets();
    
    // TODO: 成员2 需要实现
    // 根据 FIRST/FOLLOW 集合构造 LL(1) 预测分析表
    bool buildLL1ParseTable(std::string &errorMsg);

    // 调试输出（可选实现）
    void debugPrint() const;
};

// TODO: 成员1 需要实现
// 去掉字符串首尾空白字符
std::string trim(const std::string &s);

// TODO: 成员1 需要实现
// 按空格拆分产生式右部，支持带引号的终结符（如 "if", "+"）
std::vector<std::string> splitSymbols(const std::string &rhs);

