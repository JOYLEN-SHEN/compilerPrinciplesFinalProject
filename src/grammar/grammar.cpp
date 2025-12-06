#include "grammar.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <cctype>

using namespace std;

// ============================================
// 成员1 实现以下函数
// ============================================

string trim(const string& s) {
    // 去除字符串首尾的空白字符
    if (s.empty()) return s;

    size_t start = 0;
    size_t end = s.length() - 1;

    // 找到第一个非空白字符
    while (start <= end && isspace(static_cast<unsigned char>(s[start]))) {
        start++;
    }

    // 找到最后一个非空白字符
    while (end >= start && isspace(static_cast<unsigned char>(s[end]))) {
        end--;
    }

    // 如果整个字符串都是空白
    if (start > end) {
        return "";
    }

    return s.substr(start, end - start + 1);
}

vector<string> splitSymbols(const string& rhs) {
    vector<string> symbols;
    string current;
    bool inQuotes = false;

    for (size_t i = 0; i < rhs.length(); i++) {
        char c = rhs[i];

        if (c == '"') {
            if (inQuotes) {
                // 结束引号
                if (!current.empty()) {
                    symbols.push_back(current);
                    current.clear();
                }
                inQuotes = false;
            }
            else {
                // 开始引号
                if (!current.empty()) {
                    string trimmed = trim(current);
                    if (!trimmed.empty()) {
                        symbols.push_back(trimmed);
                    }
                    current.clear();
                }
                inQuotes = true;
            }
        }
        else if (isspace(c) && !inQuotes) {
            // 空白字符分隔符号（不在引号内时）
            if (!current.empty()) {
                string trimmed = trim(current);
                if (!trimmed.empty()) {
                    symbols.push_back(trimmed);
                }
                current.clear();
            }
        }
        else {
            // 普通字符
            current += c;
        }
    }

    // 处理最后一个符号
    if (!current.empty()) {
        string trimmed = trim(current);
        if (!trimmed.empty()) {
            symbols.push_back(trimmed);
        }
    }

    return symbols;
}

bool Grammar::loadFromFile(const string& path, string& errorMsg) {
    // 清空现有数据
    nonterminals.clear();
    terminals.clear();
    productions.clear();
    startSymbol.clear();
    first.clear();
    follow.clear();
    parseTable.clear();

    ifstream file(path);
    if (!file.is_open()) {
        errorMsg = "无法打开文件: " + path;
        return false;
    }

    string line;
    int lineNum = 0;

    while (getline(file, line)) {
        lineNum++;
        line = trim(line);

        // 跳过空行和注释
        if (line.empty() || line[0] == '#') {
            continue;
        }

        // 处理起始符号定义
        if (line.find("%Start") == 0) {
            size_t startPos = line.find(' ');
            if (startPos == string::npos) {
                errorMsg = "第" + to_string(lineNum) + "行: %Start 指令格式错误";
                return false;
            }

            startSymbol = trim(line.substr(startPos + 1));
            // 移除可能的分号
            if (!startSymbol.empty() && startSymbol.back() == ';') {
                startSymbol.pop_back();
                startSymbol = trim(startSymbol);
            }

            if (startSymbol.empty()) {
                errorMsg = "第" + to_string(lineNum) + "行: 起始符号不能为空";
                return false;
            }
            continue;
        }

        // 处理产生式
        size_t arrowPos = line.find("->");
        if (arrowPos == string::npos) {
            // 不是产生式，可能是其他指令或格式错误
            continue;
        }

        // 提取左部
        string lhs = trim(line.substr(0, arrowPos));
        if (lhs.empty()) {
            errorMsg = "第" + to_string(lineNum) + "行: 产生式左部为空";
            return false;
        }

        // 添加到非终结符集合
        nonterminals.insert(lhs);

        // 提取右部
        string rhsStr = trim(line.substr(arrowPos + 2));

        // 处理多个候选式（用 | 分隔）
        vector<string> alternatives;
        size_t start = 0;
        size_t pipePos;

        while ((pipePos = rhsStr.find('|', start)) != string::npos) {
            string alt = trim(rhsStr.substr(start, pipePos - start));
            if (!alt.empty()) {
                alternatives.push_back(alt);
            }
            start = pipePos + 1;
        }

        // 添加最后一个候选式
        string lastAlt = trim(rhsStr.substr(start));
        if (!lastAlt.empty()) {
            alternatives.push_back(lastAlt);
        }

        // 如果没有候选式，添加epsilon产生式
        if (alternatives.empty()) {
            Production prod;
            prod.lhs = lhs;
            prod.rhs = { EPSILON };
            productions.push_back(prod);
        }
        else {
            // 为每个候选式创建产生式
            for (const auto& alt : alternatives) {
                Production prod;
                prod.lhs = lhs;
                prod.rhs = splitSymbols(alt);

                // 处理epsilon
                if (prod.rhs.size() == 1 && trim(prod.rhs[0]) == "epsilon") {
                    prod.rhs = { EPSILON };
                }

                productions.push_back(prod);

                // 识别符号类型
                for (const auto& symbol : prod.rhs) {
                    if (symbol == EPSILON) {
                        continue;  // epsilon是特殊符号
                    }

                    // 检查是否为带引号的终结符
                    if (symbol.length() >= 2 && symbol[0] == '"' && symbol.back() == '"') {
                        string term = symbol.substr(1, symbol.length() - 2);
                        terminals.insert(term);
                    }
                    // 检查是否为非终结符（假设非终结符以大写字母开头）
                    else if (!symbol.empty() && isupper(symbol[0])) {
                        // 后续可能需要添加到nonterminals，但这里先不添加
                        // 因为可能在其他产生式左部定义
                    }
                    else {
                        // 其他符号视为终结符
                        terminals.insert(symbol);
                    }
                }
            }
        }
    }

    file.close();

    // 如果没有显式指定起始符号，使用第一个产生式的左部
    if (startSymbol.empty() && !productions.empty()) {
        startSymbol = productions[0].lhs;
    }

    // 验证文法
    if (nonterminals.empty()) {
        errorMsg = "文法中没有定义非终结符";
        return false;
    }

    if (startSymbol.empty()) {
        errorMsg = "未指定起始符号";
        return false;
    }

    if (nonterminals.find(startSymbol) == nonterminals.end()) {
        errorMsg = "起始符号 '" + startSymbol + "' 不是非终结符";
        return false;
    }

    // 确保非终结符集合包含所有出现在产生式左部的符号
    // 和所有出现在产生式右部的大写字母开头的符号
    for (const auto& prod : productions) {
        for (const auto& symbol : prod.rhs) {
            if (symbol != EPSILON && !symbol.empty() && isupper(symbol[0])) {
                // 大写字母开头的符号视为非终结符
                nonterminals.insert(symbol);
            }
        }
    }

    // 从终结符集合中移除非终结符
    for (const auto& nt : nonterminals) {
        terminals.erase(nt);
    }

    // 添加特殊符号到终结符集合
    terminals.insert(END_MARKER);

    // 输出文法信息（调试用）
    /*
    cout << "文法加载成功：" << endl;
    cout << "  起始符号: " << startSymbol << endl;
    cout << "  非终结符 (" << nonterminals.size() << "个): ";
    for (const auto& nt : nonterminals) cout << nt << " ";
    cout << endl;
    cout << "  终结符 (" << terminals.size() << "个): ";
    for (const auto& t : terminals) cout << t << " ";
    cout << endl;
    cout << "  产生式 (" << productions.size() << "个):" << endl;
    for (size_t i = 0; i < productions.size(); i++) {
        cout << "    [" << i << "] " << productions[i].lhs << " -> ";
        if (productions[i].rhs.size() == 1 && productions[i].rhs[0] == EPSILON) {
            cout << EPSILON;
        } else {
            for (const auto& sym : productions[i].rhs) {
                cout << sym << " ";
            }
        }
        cout << endl;
    }
    */

    // 调用成员2的函数（如果已实现）
    //try {
    //    computeFirstSets();
    //    computeFollowSets();
    //    if (!buildLL1ParseTable(errorMsg)) {
    //        // 如果不是LL(1)文法，可能只是警告而不是错误
    //        // 取决于具体需求
    //        cout << "警告: " << errorMsg << endl;
    //    }
    //}
    //catch (...) {
    //    // 成员2的函数可能还没实现，继续执行
    //}

    return true;
}