#include "pl0_icg.h"

#include <sstream>

// ============================================
// TODO: 成员3 实现以下函数
// ============================================

//Token &PL0ICG::peek() {
//    // TODO: 返回当前 token（不移动指针）
//    // 注意：需要检查 ip 是否越界
//    static Token dummy; // 占位
//    return dummy;
//}
//
//Token &PL0ICG::advance() {
//    // TODO: 返回当前 token 并移动指针到下一个
//    static Token dummy; // 占位
//    return dummy;
//}
//
//bool PL0ICG::accept(const std::string &type) {
//    // TODO: 如果当前 token 类型匹配，接受并移动指针
//    return false; // 占位
//}
//
//bool PL0ICG::expect(const std::string &type, std::string &errorMsg) {
//    // TODO: 期望当前 token 类型匹配
//    // 不匹配时设置错误信息（包含行号列号）并返回 false
//    return false; // 占位
//}
//
//std::string PL0ICG::newTemp() {
//    // TODO: 生成新的临时变量名（如 "t1", "t2", ...）
//    return ""; // 占位
//}
//
//std::string PL0ICG::newLabel() {
//    // TODO: 生成新的标签名（如 "L1", "L2", ...）
//    return ""; // 占位
//}
//
//bool PL0ICG::generate(const std::string &source,
//                      std::vector<std::string> &tac,
//                      std::string &errorMsg) {
//    // TODO: 实现三地址码生成入口
//    // 1. 重置状态（清空符号表、代码、计数器）
//    // 2. 调用 lexer.tokenize() 进行词法分析
//    // 3. 设置 ip = 0
//    // 4. 调用 parseProgram() 进行语法分析
//    // 5. 将生成的 code 复制到 tac
//    // 6. 返回是否成功
//    return false; // 占位
//}
//
//bool PL0ICG::parseProgram(std::string &errorMsg) {
//    // TODO: 解析程序：program ::= block "."
//    // 1. 调用 parseBlock()
//    // 2. 期望 "."
//    return false; // 占位
//}
//
//bool PL0ICG::parseBlock(std::string &errorMsg) {
//    // TODO: 解析块：block ::= [constDecl] [varDecl] "begin" statementList "end"
//    // 1. 可选解析 const 声明（识别 "const" 关键字）
//    //    - 解析 constItem（id "=" num）
//    //    - 解析 constMore（"," constItem constMore | epsilon）
//    //    - 期望 ";"
//    //    - 将常量加入符号表（isConst=true, constValue=值）
//    // 2. 可选解析 var 声明（识别 "var" 关键字）
//    //    - 解析 id
//    //    - 解析 varMore（"," id varMore | epsilon）
//    //    - 期望 ";"
//    //    - 将变量加入符号表（isConst=false）
//    // 3. 期望 "begin"
//    // 4. 解析 statementList
//    // 5. 期望 "end"
//    return false; // 占位
//}
//
//bool PL0ICG::parseStatement(std::string &errorMsg) {
//    // TODO: 解析语句
//    // statement ::= id ":=" expression
//    //            | "begin" statementList "end"
//    //            | "if" condition "then" statement
//    //            | "while" condition "do" statement
//    // 
//    // 1. 如果当前 token 是 id：
//    //    - 检查符号表中是否存在（不存在则报错）
//    //    - 期望 ":="
//    //    - 调用 parseExpression() 得到结果临时变量
//    //    - 生成赋值代码：id " = " temp
//    // 2. 如果当前 token 是 "begin"：
//    //    - 解析 statementList（递归调用 parseStatement）
//    // 3. 如果当前 token 是 "if"：
//    //    - 调用 parseCondition() 得到条件结果临时变量
//    //    - 期望 "then"
//    //    - 生成条件跳转：ifFalse condTemp goto labelElse
//    //    - 解析 then 分支的 statement
//    //    - 生成 labelElse 标签
//    // 4. 如果当前 token 是 "while"：
//    //    - 生成 labelBegin 标签
//    //    - 调用 parseCondition() 得到条件结果临时变量
//    //    - 生成条件跳转：ifFalse condTemp goto labelEnd
//    //    - 期望 "do"
//    //    - 解析循环体的 statement
//    //    - 生成跳转：goto labelBegin
//    //    - 生成 labelEnd 标签
//    return false; // 占位
//}
//
//bool PL0ICG::parseCondition(std::string &resultTemp, std::string &errorMsg) {
//    // TODO: 解析条件表达式
//    // condition ::= "odd" expression
//    //            | expression relop expression
//    // 
//    // 1. 如果当前 token 是 "odd"：
//    //    - 解析 expression
//    //    - 生成：temp = exprTemp % 2; temp2 = temp != 0
//    //    - resultTemp = temp2
//    // 2. 否则：
//    //    - 解析左表达式
//    //    - 识别关系运算符（=, #, <, <=, >, >=）
//    //    - 解析右表达式
//    //    - 生成：temp = leftTemp relop rightTemp
//    //    - resultTemp = temp
//    return false; // 占位
//}
//
//bool PL0ICG::parseExpression(std::string &resultTemp, std::string &errorMsg) {
//    // TODO: 解析表达式
//    // expression ::= ["+"|"-"] term { ("+"|"-") term }
//    // 
//    // 1. 可选解析符号（"+" 或 "-"）
//    // 2. 解析第一个 term
//    // 3. 如果有符号且是 "-"，生成：temp = 0 - termTemp; termTemp = temp
//    // 4. 循环处理后续的 "+" term 或 "-" term：
//    //    - 解析下一个 term
//    //    - 生成：newTemp = resultTemp op termTemp
//    //    - resultTemp = newTemp
//    return false; // 占位
//}
//
//bool PL0ICG::parseTerm(std::string &resultTemp, std::string &errorMsg) {
//    // TODO: 解析项
//    // term ::= factor { ("*"|"/") factor }
//    // 
//    // 1. 解析第一个 factor
//    // 2. 循环处理后续的 "*" factor 或 "/" factor：
//    //    - 解析下一个 factor
//    //    - 生成：newTemp = resultTemp op factorTemp
//    //    - resultTemp = newTemp
//    return false; // 占位
//}
//
//bool PL0ICG::parseFactor(std::string &resultTemp, std::string &errorMsg) {
//    // TODO: 解析因子
//    // factor ::= id | num | "(" expression ")"
//    // 
//    // 1. 如果当前 token 是 id：
//    //    - 检查符号表（不存在则报错）
//    //    - resultTemp = id 名称
//    // 2. 如果当前 token 是 num：
//    //    - 生成临时变量：temp = num值
//    //    - resultTemp = temp
//    // 3. 如果当前 token 是 "("：
//    //    - 解析 expression
//    //    - 期望 ")"
//    //    - resultTemp = expression 的结果临时变量
//    return false; // 占位
//}

PL0ICG::PL0ICG(const std::vector<Token>& t)
    : tokens(t), pos(0), tempCount(0), labelCount(0) {
}

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
Token PL0ICG::peek() const {
    if (pos < tokens.size()) return tokens[pos];
    return { "$", "", 0, 0 };
}

Token PL0ICG::next() {
    if (pos < tokens.size()) return tokens[pos++];
    return { "$", "", 0, 0 };
}

void PL0ICG::expect(const std::string& type, const std::string& errMsg) {
    if (next().type != type) throw std::runtime_error(errMsg);
}

std::string PL0ICG::newTemp() {
    return "t" + std::to_string(++tempCount);
}

std::string PL0ICG::newLabel() {
    return "L" + std::to_string(++labelCount);
}

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
