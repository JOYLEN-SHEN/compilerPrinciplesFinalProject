#include "ll1_parser.h"

#include <stack>
#include <sstream>
#include <algorithm>

// ============================================
// DONE: 成员2 已实现以下函数
// ============================================

bool LL1Parser::parse(const std::vector<Token> &tokens,
                      std::vector<int> &usedProductions,
                      std::string &errorMsg) const{
    // DONE: 成员2 已实现 - LL(1) 表驱动语法分析算法
    usedProductions.clear();
    
    // 1. 初始化栈：压入 $ 和开始符号
    std::stack<std::string> stack;
    stack.push(grammar.END_MARKER);
    stack.push(grammar.startSymbol);
    
    // 2. 初始化输入指针 ip = 0
    size_t ip = 0;
    
    // 3. 循环直到栈为空
    while (!stack.empty()) {
        std::string X = stack.top();
        stack.pop();
        
        // 如果输入已耗尽
        if (ip >= tokens.size()) {
            std::ostringstream oss;
            oss << "语法错误：输入已耗尽，但栈中还有符号 '" << X << "'";
            errorMsg = oss.str();
            return false;
        }
        
        std::string a = tokens[ip].type;
        
        // 如果 X == a（匹配）
        if (X == a) {
            ip++;
            continue;
        }
        
        // 如果 X 是终结符但不匹配
        if (grammar.terminals.find(X) != grammar.terminals.end()) {
            std::ostringstream oss;
            oss << "语法错误：第" << tokens[ip].line << "行，第" << tokens[ip].column << "列: "
                << "期望符号 '" << X << "'，但遇到 '" << a << "'";
            errorMsg = oss.str();
            return false;
        }
        
        // 如果 X 是非终结符
        if (grammar.nonterminals.find(X) != grammar.nonterminals.end()) {
            // 查表 M[X, a]
            auto it1 = grammar.parseTable.find(X);
            if (it1 == grammar.parseTable.end()) {
                std::ostringstream oss;
                oss << "语法错误：第" << tokens[ip].line << "行，第" << tokens[ip].column << "列: "
                    << "非终结符 '" << X << "' 在分析表中不存在";
                errorMsg = oss.str();
                return false;
            }
            
            auto it2 = it1->second.find(a);
            if (it2 == it1->second.end()) {
                std::ostringstream oss;
                oss << "语法错误：第" << tokens[ip].line << "行，第" << tokens[ip].column << "列: "
                    << "在M[" << X << ", " << a << "]处没有可用的产生式";
                errorMsg = oss.str();
                return false;
            }
            
            // 获取产生式编号
            int prodIndex = it2->second;
            if (prodIndex < 0 || prodIndex >= static_cast<int>(grammar.productions.size())) {
                std::ostringstream oss;
                oss << "内部错误：产生式编号 " << prodIndex << " 无效";
                errorMsg = oss.str();
                return false;
            }
            
            const auto& prod = grammar.productions[prodIndex];
            
            // 记录使用的产生式
            usedProductions.push_back(prodIndex);
            
            // 将产生式右部符号逆序压栈（跳过 epsilon）
            if (!prod.rhs.empty() && !(prod.rhs.size() == 1 && prod.rhs[0] == grammar.EPSILON)) {
                for (auto it = prod.rhs.rbegin(); it != prod.rhs.rend(); ++it) {
                    if (*it != grammar.EPSILON) {
                        stack.push(*it);
                    }
                }
            }
        } else {
            // X既不是终结符也不是非终结符（不应该发生）
            std::ostringstream oss;
            oss << "内部错误：未知符号 '" << X << "'";
            errorMsg = oss.str();
            return false;
        }
    }
    
    // 4. 检查输入是否完全消耗（最后一个 token 应该是 $）
    if (ip < tokens.size() - 1) {
        std::ostringstream oss;
        oss << "语法错误：第" << tokens[ip].line << "行，第" << tokens[ip].column << "列: "
            << "输入未完全消耗，还有多余的符号 '" << tokens[ip].type << "'";
        errorMsg = oss.str();
        return false;
    }
    
    if (ip == tokens.size() - 1 && tokens[ip].type != grammar.END_MARKER) {
        std::ostringstream oss;
        oss << "语法错误：第" << tokens[ip].line << "行，第" << tokens[ip].column << "列: "
            << "期望结束标记 '$'，但遇到 '" << tokens[ip].type << "'";
        errorMsg = oss.str();
        return false;
    }
    
    // 5. 返回 true
    return true;
}

