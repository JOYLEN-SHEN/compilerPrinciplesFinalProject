#include "grammar.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <cctype>

using namespace std;

// ============================================
// DONE: 成员1 已实现以下函数
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
                // 注意：splitSymbols()已经去掉了引号，所以这里不需要检查引号
                for (const auto& symbol : prod.rhs) {
                    if (symbol == EPSILON) {
                        continue;  // epsilon是特殊符号
                    }

                    // 检查是否为非终结符（假设非终结符以大写字母开头）
                    if (!symbol.empty() && isupper(symbol[0])) {
                        // 后续可能需要添加到nonterminals，但这里先不添加
                        // 因为可能在其他产生式左部定义
                    }
                    else {
                        // 其他符号视为终结符（包括splitSymbols()从引号中提取的符号）
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

    // 调用成员2的函数计算FIRST/FOLLOW集合并构造分析表
    computeFirstSets();
    computeFollowSets();
    if (!buildLL1ParseTable(errorMsg)) {
        // 如果不是LL(1)文法，返回错误
        return false;
    }

    return true;
}

// ============================================
// DONE: 成员2 已实现以下函数
// ============================================

void Grammar::computeFirstSets() {
    // DONE: 成员2 已实现 - 计算所有符号的 FIRST 集合
    // 使用迭代不动点算法
    
    // 初始化：所有终结符的FIRST集合是它自己
    for (const auto& term : terminals) {
        first[term].insert(term);
    }
    
    // 初始化：所有非终结符的FIRST集合为空
    for (const auto& nt : nonterminals) {
        first[nt] = set<string>();
    }
    
    // 迭代不动点算法计算FIRST集合
    bool changed = true;
    while (changed) {
        changed = false;
        
        // 遍历所有产生式
        for (const auto& prod : productions) {
            const string& lhs = prod.lhs;
            const vector<string>& rhs = prod.rhs;
            
            // 如果产生式右部为空（epsilon产生式）
            if (rhs.empty() || (rhs.size() == 1 && rhs[0] == EPSILON)) {
                // 将epsilon添加到FIRST(lhs)
                if (first[lhs].find(EPSILON) == first[lhs].end()) {
                    first[lhs].insert(EPSILON);
                    changed = true;
                }
                continue;
            }
            
            // 计算产生式右部的FIRST集合
            bool allHaveEpsilon = true;
            for (size_t i = 0; i < rhs.size(); i++) {
                const string& symbol = rhs[i];
                
                // 将FIRST(symbol) - {epsilon} 添加到FIRST(lhs)
                for (const auto& firstSym : first[symbol]) {
                    if (firstSym != EPSILON) {
                        if (first[lhs].find(firstSym) == first[lhs].end()) {
                            first[lhs].insert(firstSym);
                            changed = true;
                        }
                    }
                }
                
                // 如果当前符号的FIRST集合不包含epsilon，停止
                if (first[symbol].find(EPSILON) == first[symbol].end()) {
                    allHaveEpsilon = false;
                    break;
                }
            }
            
            // 如果所有符号的FIRST集合都包含epsilon，将epsilon添加到FIRST(lhs)
            if (allHaveEpsilon) {
                if (first[lhs].find(EPSILON) == first[lhs].end()) {
                    first[lhs].insert(EPSILON);
                    changed = true;
                }
            }
        }
    }
}

void Grammar::computeFollowSets() {
    // DONE: 成员2 已实现 - 计算所有非终结符的 FOLLOW 集合
    // 使用迭代不动点算法
    
    // 初始化：所有非终结符的FOLLOW集合为空
    for (const auto& nt : nonterminals) {
        follow[nt] = set<string>();
    }
    
    // 将$添加到FOLLOW(起始符号)
    follow[startSymbol].insert(END_MARKER);
    
    // 迭代不动点算法计算FOLLOW集合
    bool changed = true;
    while (changed) {
        changed = false;
        
        // 遍历所有产生式 A -> αBβ
        for (const auto& prod : productions) {
            const string& A = prod.lhs;
            const vector<string>& rhs = prod.rhs;
            
            // 遍历产生式右部的每个符号
            for (size_t i = 0; i < rhs.size(); i++) {
                const string& B = rhs[i];
                
                // 如果B是非终结符
                if (nonterminals.find(B) != nonterminals.end()) {
                    // 计算β的FIRST集合
                    set<string> firstBeta;
                    bool betaHasEpsilon = true;
                    
                    for (size_t j = i + 1; j < rhs.size(); j++) {
                        const string& symbol = rhs[j];
                        
                        // 将FIRST(symbol) - {epsilon} 添加到FIRST(β)
                        for (const auto& firstSym : first[symbol]) {
                            if (firstSym != EPSILON) {
                                firstBeta.insert(firstSym);
                            }
                        }
                        
                        // 如果当前符号的FIRST集合不包含epsilon，停止
                        if (first[symbol].find(EPSILON) == first[symbol].end()) {
                            betaHasEpsilon = false;
                            break;
                        }
                    }
                    
                    // 将FIRST(β) - {epsilon} 添加到FOLLOW(B)
                    for (const auto& sym : firstBeta) {
                        if (follow[B].find(sym) == follow[B].end()) {
                            follow[B].insert(sym);
                            changed = true;
                        }
                    }
                    
                    // 如果β可以推导出epsilon（或β为空），将FOLLOW(A)添加到FOLLOW(B)
                    if (betaHasEpsilon) {
                        for (const auto& sym : follow[A]) {
                            if (follow[B].find(sym) == follow[B].end()) {
                                follow[B].insert(sym);
                                changed = true;
                            }
                        }
                    }
                }
            }
        }
    }
}

bool Grammar::buildLL1ParseTable(string& errorMsg) {
    // DONE: 成员2 已实现 - 根据 FIRST/FOLLOW 集合构造 LL(1) 预测分析表
    // 清空分析表
    parseTable.clear();
    
    // 为每个非终结符初始化分析表
    for (const auto& nt : nonterminals) {
        parseTable[nt] = map<string, int>();
    }
    
    // 遍历所有产生式，填充分析表
    for (size_t i = 0; i < productions.size(); i++) {
        const auto& prod = productions[i];
        const string& A = prod.lhs;
        const vector<string>& rhs = prod.rhs;
        
        // 计算产生式右部的FIRST集合
        set<string> firstRhs;
        bool rhsHasEpsilon = false;
        
        if (rhs.empty() || (rhs.size() == 1 && rhs[0] == EPSILON)) {
            // epsilon产生式
            rhsHasEpsilon = true;
        } else {
            // 计算FIRST(α)，其中α是产生式右部
            bool allHaveEpsilon = true;
            for (const auto& symbol : rhs) {
                // 将FIRST(symbol) - {epsilon} 添加到FIRST(α)
                for (const auto& firstSym : first[symbol]) {
                    if (firstSym != EPSILON) {
                        firstRhs.insert(firstSym);
                    }
                }
                
                // 如果当前符号的FIRST集合不包含epsilon，停止
                if (first[symbol].find(EPSILON) == first[symbol].end()) {
                    allHaveEpsilon = false;
                    break;
                }
            }
            
            if (allHaveEpsilon) {
                rhsHasEpsilon = true;
            }
        }
        
        // 对于FIRST(α)中的每个终结符a，将产生式i添加到M[A, a]
        for (const auto& a : firstRhs) {
            if (parseTable[A].find(a) != parseTable[A].end()) {
                // 冲突：表项已存在
                ostringstream oss;
                oss << "LL(1)冲突：在M[" << A << ", " << a << "]处，"
                    << "产生式" << parseTable[A][a] << "和产生式" << i << "冲突";
                errorMsg = oss.str();
                return false;
            }
            parseTable[A][a] = static_cast<int>(i);
        }
        
        // 如果epsilon在FIRST(α)中，对于FOLLOW(A)中的每个终结符b，将产生式i添加到M[A, b]
        if (rhsHasEpsilon) {
            for (const auto& b : follow[A]) {
                if (parseTable[A].find(b) != parseTable[A].end()) {
                    // 冲突：表项已存在
                    ostringstream oss;
                    oss << "LL(1)冲突：在M[" << A << ", " << b << "]处，"
                        << "产生式" << parseTable[A][b] << "和产生式" << i << "冲突";
                    errorMsg = oss.str();
                    return false;
                }
                parseTable[A][b] = static_cast<int>(i);
            }
        }
    }
    
    return true;
}