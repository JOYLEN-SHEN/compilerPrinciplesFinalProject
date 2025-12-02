#include "SyntaxTree.h"
#include "../lex/Token.h"

#include <vector>
#include <string>
#include <sstream>

using std::string;
using std::vector;

// 递归下降 LL(1) Parser 的简单实现
// 文法参考 README 与 pl0_syn_rules.txt：
//   program   -> stmt_list
//   stmt_list -> stmt { ; stmt } [;]
//   stmt      -> assign | if | block
//   assign    -> id := expr
//   if        -> if expr then stmt
//   block     -> begin stmt_list end
//   expr      -> add_expr [ '>' add_expr ]
//   add_expr  -> term { (+|-) term }
//   term      -> id | number
class Parser {
public:
    Parser(const vector<Token> &toks, vector<string> &errs)
        : tokens(toks), pos(0), errors(errs) {}

    // program -> stmt_list EOF
    SyntaxNode* parseProgram() {
        SyntaxNode *prog = new SyntaxNode(NodeType::Program);
        SyntaxNode *stmts = parseStmtList();
        prog->children.push_back(stmts);
        expect(TokenType::EndOfFile, "程序末尾应为 EOF");
        return prog;
    }

private:
    const vector<Token> &tokens;
    std::size_t pos;
    vector<string> &errors;

    const Token &peek() const {
        if (pos >= tokens.size()) {
            static Token eof(TokenType::EndOfFile, "", 0);
            return eof;
        }
        return tokens[pos];
    }

    const Token &get() {
        const Token &t = peek();
        if (pos < tokens.size()) {
            ++pos;
        }
        return t;
    }

    bool match(TokenType expected) {
        if (peek().type == expected) {
            get();
            return true;
        }
        return false;
    }

    void reportError(const string &msg) {
        const Token &t = peek();
        std::ostringstream oss;
        oss << "第 " << t.line << " 行：语法错误，" << msg;
        errors.push_back(oss.str());
    }

    void expect(TokenType expected, const string &msg) {
        if (!match(expected)) {
            reportError(msg);
        }
    }

    // stmt_list -> stmt { ; stmt } [;]
    SyntaxNode* parseStmtList() {
        SyntaxNode *list = new SyntaxNode(NodeType::StmtList);

        // 至少解析一个语句
        SyntaxNode *first = parseStmt();
        list->children.push_back(first);

        while (match(TokenType::Semicolon)) {
            // 可选尾分号：如果后面直接是 end 或 EOF，则不再读取新的 stmt
            TokenType t = peek().type;
            if (t == TokenType::KeywordEnd || t == TokenType::EndOfFile) {
                break;
            }
            list->children.push_back(parseStmt());
        }
        return list;
    }

    // stmt -> assign | if | block
    SyntaxNode* parseStmt() {
        const Token &tk = peek();
        switch (tk.type) {
        case TokenType::Identifier:
            return parseAssign();
        case TokenType::KeywordIf:
            return parseIf();
        case TokenType::KeywordBegin:
            return parseBlock();
        default:
            reportError("无法识别的语句");
            // 简单同步恢复：丢弃当前 token，返回一个空语句节点
            get();
            return new SyntaxNode(NodeType::StmtList);
        }
    }

    // assign -> id := expr
    SyntaxNode* parseAssign() {
        const Token &idTok = peek();
        if (!match(TokenType::Identifier)) {
            reportError("赋值语句应以标识符开头");
            // 同步：丢弃一个 token，返回空节点
            get();
            return new SyntaxNode(NodeType::AssignStmt);
        }

        expect(TokenType::Assign, "赋值语句缺少 ':='");

        SyntaxNode *node = new SyntaxNode(NodeType::AssignStmt, idTok);
        SyntaxNode *rhs = parseExpr();
        node->children.push_back(rhs);
        return node;
    }

    // if -> if expr then stmt
    SyntaxNode* parseIf() {
        const Token &kw = peek();
        expect(TokenType::KeywordIf, "应为 'if'");

        SyntaxNode *node = new SyntaxNode(NodeType::IfStmt, kw);
        SyntaxNode *cond = parseExpr();
        node->children.push_back(cond);

        expect(TokenType::KeywordThen, "if 条件后应为 'then'");

        SyntaxNode *body = parseStmt();
        node->children.push_back(body);
        return node;
    }

    // block -> begin stmt_list end
    SyntaxNode* parseBlock() {
        const Token &kw = peek();
        expect(TokenType::KeywordBegin, "应为 'begin'");

        SyntaxNode *node = new SyntaxNode(NodeType::BlockStmt, kw);
        SyntaxNode *body = parseStmtList();
        node->children.push_back(body);

        expect(TokenType::KeywordEnd, "缺少 'end'");
        return node;
    }

    // expr -> add_expr [ '>' add_expr ]
    SyntaxNode* parseExpr() {
        SyntaxNode *left = parseAddExpr();
        if (peek().type == TokenType::Greater) {
            Token op = get();
            SyntaxNode *rel = new SyntaxNode(NodeType::RelExpr, op);
            rel->children.push_back(left);
            rel->children.push_back(parseAddExpr());
            return rel;
        }
        return left;
    }

    // add_expr -> term { (+|-) term }
    SyntaxNode* parseAddExpr() {
        // 构造左结合的表达式树：((t1 + t2) - t3) ...
        SyntaxNode *left = parseTerm();

        while (peek().type == TokenType::Plus || peek().type == TokenType::Minus) {
            Token op = get();
            SyntaxNode *exprNode = new SyntaxNode(NodeType::Expr, op);
            exprNode->children.push_back(left);
            exprNode->children.push_back(parseTerm());
            left = exprNode;
        }
        return left;
    }

    // term -> id | number
    SyntaxNode* parseTerm() {
        const Token &tk = peek();
        if (tk.type == TokenType::Identifier || tk.type == TokenType::Number) {
            get();
            return new SyntaxNode(NodeType::Term, tk);
        }

        reportError("表达式缺少操作数");
        // 出错时丢弃当前 token，返回一个占位节点，避免死循环
        get();
        return new SyntaxNode(NodeType::Term);
    }
};

// 分析入口：根据 Token 序列构建语法树
SyntaxNode* runParser(const vector<Token> &tokens, vector<string> &errors) {
    Parser p(tokens, errors);
    return p.parseProgram();
}