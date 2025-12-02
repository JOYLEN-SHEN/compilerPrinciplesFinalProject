#pragma once

#include <vector>
#include <string>
#include "../lex/Token.h"

// 语法树节点类型（可根据课程扩展）
enum class NodeType {
    Program,
    StmtList,
    AssignStmt,
    IfStmt,
    BlockStmt,
    Expr,      // 加减表达式
    RelExpr,   // 关系表达式（例如 >）
    Term
};

struct SyntaxNode {
    NodeType type;
    Token token;                     // 关键 token（如标识符 / 运算符）
    std::vector<SyntaxNode*> children;

    explicit SyntaxNode(NodeType t) : type(t) {}
    SyntaxNode(NodeType t, const Token &tok) : type(t), token(tok) {}
};

// Done: 递归释放整棵语法树
inline void freeSyntaxTree(SyntaxNode *node) {
    if (!node) return;
    for (auto *ch : node->children) {
        freeSyntaxTree(ch);
    }
    delete node;
}



