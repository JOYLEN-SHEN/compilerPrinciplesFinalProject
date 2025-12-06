#include "lexer.h"
#include <cctype>
#include <sstream>
#include <algorithm>
#include <iostream>

using namespace std;

// ============================================
// 成员1 实现以下函数
// ============================================

void Lexer::configureFromTerminals(const set<string>& terminals) {
    // 清空现有配置
    keywordTokens.clear();
    symbolTokens.clear();
    hasIdToken = false;
    hasNumToken = false;

    // 遍历终结符集合
    for (const auto& term : terminals) {
        // 检查是否为特殊符号
        if (term == "id") {
            hasIdToken = true;
        }
        else if (term == "num") {
            hasNumToken = true;
        }
        // 检查是否为纯字母（关键字）
        else if (all_of(term.begin(), term.end(), [](char c) {
            return isalpha(c);
            })) {
            // 纯字母，视为关键字
            keywordTokens.insert(term);
        }
        else {
            // 其他符号（运算符、分隔符等）
            symbolTokens.insert(term);
        }
    }

    // 输出调试信息（可选）
    /*
    cout << "词法分析器配置完成：" << endl;
    cout << "  关键字: ";
    for (const auto& kw : keywordTokens) cout << kw << " ";
    cout << endl << "  符号: ";
    for (const auto& sym : symbolTokens) cout << sym << " ";
    cout << endl << "  标识符识别: " << (hasIdToken ? "启用" : "禁用") << endl;
    cout << "  数字识别: " << (hasNumToken ? "启用" : "禁用") << endl;
    */
}

Lexer Lexer::createPL0Lexer() {
    Lexer lx;

    // 设置PL/0关键字
    set<string> pl0Keywords = {
        "const", "var", "begin", "end", "if", "then",
        "while", "do", "odd", "procedure", "call", "return"
    };

    // 设置PL/0运算符和分隔符
    set<string> pl0Symbols = {
        ":=", "+", "-", "*", "/",
        "=", "#", "<", "<=", ">", ">=",
        "(", ")", ",", ".", ";"
    };

    // 合并配置
    set<string> terminals;
    terminals.insert(pl0Keywords.begin(), pl0Keywords.end());
    terminals.insert(pl0Symbols.begin(), pl0Symbols.end());
    terminals.insert("id");
    terminals.insert("num");

    // 配置词法分析器
    lx.configureFromTerminals(terminals);

    return lx;
}

bool Lexer::isIdentifierStart(char c) const {
    // 标识符开头：字母或下划线
    return isalpha(c) || c == '_';
}

bool Lexer::isIdentifierChar(char c) const {
    // 标识符字符：字母、数字或下划线
    return isalnum(c) || c == '_';
}

bool Lexer::tokenize(const string& source,
    vector<Token>& outTokens,
    string& errorMsg) {
    outTokens.clear();

    int line = 1;        // 当前行号（从1开始）
    int column = 1;      // 当前列号（从1开始）
    size_t pos = 0;      // 当前位置
    size_t lineStartPos = 0;  // 当前行的开始位置

    while (pos < source.length()) {
        char currentChar = source[pos];
        int tokenLine = line;
        int tokenColumn = column;

        // 跳过空白字符
        if (isspace(currentChar)) {
            if (currentChar == '\n') {
                line++;
                column = 1;
                lineStartPos = pos + 1;
            }
            else {
                column++;
            }
            pos++;
            continue;
        }

        // 处理注释（PL/0使用//注释）
        if (currentChar == '/' && pos + 1 < source.length() && source[pos + 1] == '/') {
            // 跳过整行注释
            while (pos < source.length() && source[pos] != '\n') {
                pos++;
                column++;
            }
            continue;
        }

        // 尝试识别标识符或关键字
        if (hasIdToken && isIdentifierStart(currentChar)) {
            size_t start = pos;

            // 读取完整的标识符
            while (pos < source.length() && isIdentifierChar(source[pos])) {
                pos++;
                column++;
            }

            string lexeme = source.substr(start, pos - start);
            Token token;
            token.lexeme = lexeme;
            token.line = tokenLine;
            token.column = tokenColumn;

            // 检查是否为关键字
            if (keywordTokens.find(lexeme) != keywordTokens.end()) {
                token.type = lexeme;  // 关键字类型就是它本身
            }
            else {
                token.type = "id";
            }

            outTokens.push_back(token);
            continue;
        }

        // 尝试识别数字
        if (hasNumToken && isdigit(currentChar)) {
            size_t start = pos;

            // 读取完整的数字序列
            while (pos < source.length() && isdigit(source[pos])) {
                pos++;
                column++;
            }

            Token token;
            token.type = "num";
            token.lexeme = source.substr(start, pos - start);
            token.line = tokenLine;
            token.column = tokenColumn;

            outTokens.push_back(token);
            continue;
        }

        // 尝试识别运算符或分隔符（最长匹配）
        bool symbolMatched = false;
        string matchedSymbol;

        // 从最长可能开始尝试匹配（最长匹配原则）
        for (size_t len = min((size_t)3, source.length() - pos); len >= 1; len--) {
            string candidate = source.substr(pos, len);
            if (symbolTokens.find(candidate) != symbolTokens.end()) {
                matchedSymbol = candidate;
                symbolMatched = true;
                break;
            }
        }

        if (symbolMatched) {
            Token token;
            token.type = matchedSymbol;
            token.lexeme = matchedSymbol;
            token.line = tokenLine;
            token.column = tokenColumn;

            outTokens.push_back(token);
            pos += matchedSymbol.length();
            column += matchedSymbol.length();
            continue;
        }

        // 无法识别的字符
        ostringstream errorStream;
        errorStream << "第" << line << "行，第" << column << "列: "
            << "无法识别的字符 '" << currentChar << "'";
        errorMsg = errorStream.str();
        return false;
    }

    // 添加结束标记 "$"
    Token eofToken;
    eofToken.type = "$";
    eofToken.lexeme = "$";
    eofToken.line = line;
    eofToken.column = column;
    outTokens.push_back(eofToken);

    return true;
}