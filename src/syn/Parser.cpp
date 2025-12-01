#include "SyntaxTree.h"
#include "../lex/Token.h"

#include <vector>
#include <string>

using std::string;
using std::vector;

// TODO: 可以仿照递归下降写一个 Parser 类

// 分析入口：根据 Token 序列构建语法树
SyntaxNode* runParser(const vector<Token> &tokens, vector<string> &errors) {
    // TODO:
    // 1. 实现一个递归下降分析器，支持 README / 规则文件中的 PL/0 子集语法
    // 2. 正确处理赋值语句、if-then、begin-end 语句块和表达式
    // 3. 对语法错误（缺少 then / end / 分号等）在 errors 中记录清晰的提示
    (void)tokens;
    (void)errors;
    return nullptr;
}



