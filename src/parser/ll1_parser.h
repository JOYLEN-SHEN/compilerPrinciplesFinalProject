#pragma once

#include "../grammar/grammar.h"
#include "../lexer/lexer.h"

#include <string>
#include <vector>

// 第 1 部分使用的 LL(1) 表驱动语法分析器

class LL1Parser {
public:
    explicit LL1Parser(const Grammar &g) : grammar(g) {}

    // DONE: 成员2 已实现
    // LL(1) 表驱动语法分析
    // 实现说明：
    // - 使用栈进行预测分析（压入 $ 和开始符号）
    // - 根据当前栈顶符号和输入符号查表选择产生式
    // - 记录使用的产生式编号序列
    // - 遇到错误时返回 false 并设置错误信息（包含行号列号）
    // - 成功时返回 true，usedProductions 包含产生式编号序列
    // - 支持终结符匹配、非终结符展开、epsilon 产生式处理
    bool parse(const std::vector<Token> &tokens,
               std::vector<int> &usedProductions,
               std::string &errorMsg) const;

private:
    const Grammar &grammar;
};

