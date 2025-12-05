#include "ll1_parser.h"

#include <stack>
#include <sstream>

// ============================================
// TODO: 成员2 实现以下函数
// ============================================

bool LL1Parser::parse(const std::vector<Token> &tokens,
                      std::vector<int> &usedProductions,
                      std::string &errorMsg) {
    // TODO: 实现 LL(1) 表驱动语法分析
    // 算法：
    // 1. 初始化栈：压入 $ 和开始符号
    // 2. 初始化输入指针 ip = 0
    // 3. 循环直到栈为空：
    //    - 设 X = 栈顶，a = tokens[ip].type
    //    - 如果 X == a（匹配）：
    //      * 弹出栈顶，ip++
    //    - 如果 X 是终结符但不匹配：
    //      * 返回 false，错误信息包含期望的符号和实际符号
    //    - 如果 X 是非终结符：
    //      * 查表 M[X, a]
    //      * 如果表项为空，返回 false（语法错误）
    //      * 如果表项为产生式编号 i：
    //        - 记录 i 到 usedProductions
    //        - 弹出栈顶
    //        - 将产生式右部符号逆序压栈（跳过 epsilon）
    // 4. 检查输入是否完全消耗（最后一个 token 应该是 $）
    // 5. 返回 true
    
    usedProductions.clear();
    // TODO: 实现分析逻辑
    return false; // 占位
}

