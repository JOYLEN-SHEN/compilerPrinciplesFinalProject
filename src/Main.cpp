#include "Common.h"
#include "Config.h"
#include "lex/Token.h"
#include "syn/SyntaxTree.h"
#include "ir/ThreeAddr.h"

#include <iostream>
#include <vector>
#include <string>

#ifdef _WIN32
#include <windows.h>
#endif

// 外部接口声明（由同学在对应模块实现）
std::vector<Token> runLexer(const std::string &source, std::vector<std::string> &errors);
SyntaxNode* runParser(const std::vector<Token> &tokens, std::vector<std::string> &errors);
std::vector<ThreeAddrCode> runCodeGen(SyntaxNode *root);
void printIR(const std::vector<ThreeAddrCode> &ir, std::ostream &os);

int main(int argc, char *argv[]) {
    #ifdef _WIN32
    // 将控制台输入 / 输出编码切换为 UTF-8，避免中文乱码
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    if (argc < 4) {
        std::cerr << "用法：exam_compiler <lex_rules> <syn_rules> <source_file>\n";
        return 1;
    }

    const std::string lexRulePath = argv[1];
    const std::string synRulePath = argv[2];
    const std::string sourcePath  = argv[3];

    std::vector<std::string> errors;

    // TODO: 根据形式化描述（词法 / 语法规则）加载配置
    if (!loadLexRules(lexRulePath, g_lexConfig, errors)) {
        for (const auto &e : errors) {
            std::cerr << e << "\n";
        }
        return 1;
    }
    if (!loadSynRules(synRulePath, g_synConfig, errors)) {
        for (const auto &e : errors) {
            std::cerr << e << "\n";
        }
        return 1;
    }

    std::string source = readFileAsString(sourcePath);
    if (source.empty()) {
        std::cerr << "无法读取源文件：" << sourcePath << "\n";
        return 1;
    }

    // 1. 词法分析
    auto tokens = runLexer(source, errors);
    bool hasLexErr = false;
    for (const auto &e : errors) {
        std::cerr << e << "\n";
        if (e.find("非法字符") != std::string::npos) {
            hasLexErr = true;
        }
    }
    if (hasLexErr) return 1;

    // 2. 语法分析
    SyntaxNode *root = runParser(tokens, errors);
    bool hasSynErr = false;
    for (const auto &e : errors) {
        if (e.find("语法错误") != std::string::npos) {
            hasSynErr = true;
        }
    }
    if (hasSynErr) {
        for (const auto &e : errors) {
            std::cerr << e << "\n";
        }
        freeSyntaxTree(root);
        return 1;
    }

    // 3. 中间代码生成
    auto ir = runCodeGen(root);
    freeSyntaxTree(root);

    // TODO: 将 IR 写入 ir.txt 文件
    std::cout << "TODO: 请在此处将生成的三地址码写入 ir.txt 并在终端提示完成信息。\n";

    return 0;
}



