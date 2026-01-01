#include "pl0_icg.h"

#include <stdexcept>

// ============================================
// DONE: 成员3 已实现以下函数
// ============================================

// PL/0 中间代码生成器构造函数
PL0ICG::PL0ICG(const std::vector<Token>& t)
    : tokens(t), pos(0), tempCount(0), labelCount(0) {
}

// PL/0 程序的三地址码生成主入口
bool PL0ICG::generate(std::vector<std::string>& outTac, std::string& error) {
    try {
        parseProgram();
        outTac = this->tac;
        return true;
    }
    catch (const std::runtime_error& e) {
        error = e.what();
        return false;
    }
}

// --- 辅助工具实现 ---

// 查看当前 token（不移动解析位置）
Token PL0ICG::peek() const {
    if (pos < tokens.size()) return tokens[pos];
    return { "$", "", 0, 0 };
}

// 读取当前 token 并前进解析位置
Token PL0ICG::next() {
    if (pos < tokens.size()) return tokens[pos++];
    return { "$", "", 0, 0 };
}

// 期望当前 token 类型匹配，否则抛出异常
void PL0ICG::expect(const std::string& type, const std::string& errMsg) {
    if (next().type != type) throw std::runtime_error(errMsg);
}

// 生成新的临时变量名
std::string PL0ICG::newTemp() {
    return "t" + std::to_string(++tempCount);
}

// 生成新的标签名
std::string PL0ICG::newLabel() {
    return "L" + std::to_string(++labelCount);
}

// 发出三地址码指令
void PL0ICG::emit(const std::string& code) {
    tac.push_back(code);
}

// --- 递归下降逻辑 ---

// program ::= block "."
void PL0ICG::parseProgram() {
    parseBlock();
    expect(".", "缺少程序结束符 '.'");
}

// block ::= [const...] [var...] statement
void PL0ICG::parseBlock() {
    if (peek().type == "const") {
        next();
        do {
            Token id = next();
            expect("=", "常量定义缺少 '='");
            Token val = next();
            symbolTable[id.lexeme] = { "const", val.lexeme };
            if (peek().type == ",") next(); else break;
        } while (true);
        expect(";", "常量声明缺少 ';'");
    }
    if (peek().type == "var") {
        next();
        do {
            Token id = next();
            symbolTable[id.lexeme] = { "var", "" };
            if (peek().type == ",") next(); else break;
        } while (true);
        expect(";", "变量声明缺少 ';'");
    }
    parseStatement();
}

// statement ::= id := expr | begin...end | if...then | while...do
void PL0ICG::parseStatement() {
    Token t = peek();
    if (t.type == "id") {
        std::string target = next().lexeme;
        if (symbolTable.find(target) == symbolTable.end()) {
            throw std::runtime_error("语义错误：变量 '" + target + "' 未定义 (行 " + std::to_string(t.line) + ")");
        }
        if (symbolTable.count(target) && symbolTable[target].type == "const")
            throw std::runtime_error("不能为常量 " + target + " 赋值");
        expect(":=", "赋值语句缺少 ':='");
        std::string src = parseExpression();
        emit(target + " = " + src);
    }
    else if (t.type == "begin") {
        next();
        parseStatement();
        while (peek().type == ";") {
            next();
            parseStatement();
        }
        expect("end", "begin 缺少对应的 end");
    }
    else if (t.type == "if") {
        next();
        std::string cond = parseCondition();
        std::string L_after = newLabel();
        emit("ifFalse " + cond + " goto " + L_after);
        expect("then", "if 缺少 then");
        parseStatement();
        emit(L_after + ":");
    }
    else if (t.type == "while") {
        next();
        std::string L_start = newLabel();
        std::string L_end = newLabel();
        emit(L_start + ":");
        std::string cond = parseCondition();
        emit("ifFalse " + cond + " goto " + L_end);
        expect("do", "while 缺少 do");
        parseStatement();
        emit("goto " + L_start);
        emit(L_end + ":");
    }
}

// condition ::= expression relop expression
std::string PL0ICG::parseCondition() {
    if (peek().type == "odd") {
        next();
        std::string expr = parseExpression();
        std::string res = newTemp();
        emit(res + " = odd " + expr);
        return res;
    }
    std::string left = parseExpression();
    std::string op = next().lexeme; // =, #, <, <=, >, >=
    std::string right = parseExpression();
    std::string res = newTemp();
    emit(res + " = " + left + " " + op + " " + right);
    return res;
}

// expression ::= ["+"|"-"] term { ("+"|"-") term }
std::string PL0ICG::parseExpression() {
    std::string prefix = "";
    if (peek().lexeme == "+" || peek().lexeme == "-") {
        prefix = next().lexeme;
    }
    std::string left = parseTerm();
    if (prefix == "-") {
        std::string t = newTemp();
        emit(t + " = -" + left);
        left = t;
    }
    while (peek().lexeme == "+" || peek().lexeme == "-") {
        std::string op = next().lexeme;
        std::string right = parseTerm();
        std::string target = newTemp();
        emit(target + " = " + left + " " + op + " " + right);
        left = target;
    }
    return left;
}

std::string PL0ICG::parseTerm() {
    std::string left = parseFactor();
    while (peek().lexeme == "*" || peek().lexeme == "/") {
        std::string op = next().lexeme;
        std::string right = parseFactor();
        std::string target = newTemp();
        emit(target + " = " + left + " " + op + " " + right);
        left = target;
    }
    return left;
}

std::string PL0ICG::parseFactor() {
    Token t = next();
    if (t.type == "num") return t.lexeme;
    if (t.type == "id") {
        // 如果是常量，直接返回其值（常量折叠优化）
        if (symbolTable.count(t.lexeme) && symbolTable[t.lexeme].type == "const") {
            return symbolTable[t.lexeme].value;
        }
        return t.lexeme;
    }
    if (t.lexeme == "(") {
        std::string res = parseExpression();
        expect(")", "缺少 ')'");
        return res;
    }
    throw std::runtime_error("非法的因子: " + t.lexeme);
}
