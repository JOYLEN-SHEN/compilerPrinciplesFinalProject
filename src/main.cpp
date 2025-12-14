#include "grammar/grammar.h"
#include "lexer/lexer.h"
#include "parser/ll1_parser.h"
#include "pl0/pl0_icg.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <locale>
#include <filesystem>

#ifdef _WIN32
#include <windows.h>
#endif

void setupConsole() {
#ifdef _WIN32
    // Windows PowerShell 默认为 ANSI，需要将编码切到 UTF-8
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    try {
        std::locale::global(std::locale(""));
    } catch (...) {
        // 如果系统 locale 不支持，忽略即可
    }
}

std::string readFileToString(const std::string &path) {
    std::ifstream in(path);
    std::ostringstream ss;
    ss << in.rdbuf();
    return ss.str();
}

std::string dumpTacToFile(const std::string &sourceFile,
                          const std::vector<std::string> &tacLines) {
    namespace fs = std::filesystem;
    fs::path outDir = "output";
    std::error_code ec;
    fs::create_directories(outDir, ec);
    fs::path srcPath(sourceFile);
    std::string stem = srcPath.stem().string();
    if (stem.empty()) stem = "program";
    fs::path outPath = outDir / (stem + "_tac.txt");
    
    // 使用 UTF-8 编码写入文件（Windows 下需要添加 BOM）
    std::ofstream out(outPath, std::ios::binary);
#ifdef _WIN32
    // 写入 UTF-8 BOM
    unsigned char bom[] = {0xEF, 0xBB, 0xBF};
    out.write(reinterpret_cast<const char*>(bom), 3);
#endif
    for (const auto &line : tacLines) {
        out << line << "\n";
    }
    return outPath.string();
}

void printTokens(const std::vector<Token>& tokens) {
    std::cout << "Token 流如下：\n";
    for (size_t i = 0; i < tokens.size(); ++i) {
        const auto& t = tokens[i];
        std::cout << i << ": "
            << "type=" << t.type
            << ", value=\"" << t.lexeme << "\""
            << ", line=" << t.line
            << ", col=" << t.column
            << "\n";
    }
}

void runPart1(const std::string &grammarFile, const std::string &sourceFile) {
    std::cout << "=== 第 1 部分：自动生成 词法分析器 + 语法分析器 ===\n";
    std::cout << "文法文件: " << grammarFile << "\n";
    std::cout << "源程序  : " << sourceFile << "\n\n";

    Grammar g;
    std::string error;
    if (!g.loadFromFile(grammarFile, error)) {
        std::cerr << "加载文法失败： " << error << "\n";
        return;
    }

    Lexer lx;
    lx.configureFromTerminals(g.terminals);

    std::string source = readFileToString(sourceFile);
    std::vector<Token> tokens;
    if (!lx.tokenize(source, tokens, error)) {
        std::cerr << "词法分析出错： " << error << "\n";
        return;
    }
    printTokens(tokens);

    LL1Parser parser(g);
    std::vector<int> usedProductions;
    if (!parser.parse(tokens, usedProductions, error)) {
        std::cerr << "语法分析出错： " << error << "\n";
        return;
    }

    std::cout << "输入程序是 **语法合法** 的。\n";
    std::cout << "分析过程中使用到的产生式序列（按应用顺序给出编号和产生式）：\n";
    for (int idx : usedProductions) {
        const auto &p = g.productions[idx];
        std::cout << idx << ": " << p.lhs << " -> ";
        if (p.rhs.empty()) {
            std::cout << g.EPSILON;
        } else {
            for (const auto &s : p.rhs) std::cout << s << " ";
        }
        std::cout << "\n";
    }
}

//void runPart2(const std::string &sourceFile) {
//    std::cout << "=== 第 2 部分：PL/0 子集的三地址码生成 ===\n";
//    std::cout << "源程序  : " << sourceFile << "\n\n";
//
//    std::string source = readFileToString(sourceFile);
//    PL0ICG icg;
//    std::vector<std::string> tac;
//    std::string error;
//    if (!icg.generate(source, tac, error)) {
//        std::cerr << "中间代码生成失败： " << error << "\n";
//        return;
//    }
//
//    std::cout << "生成的三地址中间代码如下：\n";
//    for (const auto &line : tac) {
//        std::cout << line << "\n";
//    }
//    std::string outPath = dumpTacToFile(sourceFile, tac);
//    std::cout << "\n已将三地址码保存到文件: " << outPath << "\n";
//}



void runPart2(const std::string& grammarFile, const std::string& sourceFile) {
    std::cout << "=== 第 2 部分：PL/0 子集的三地址码生成 ===\n";

    // 1-4 步骤保持不变（加载文法、读取源码、词法分析）
    Grammar g;
    std::string error;
    if (!g.loadFromFile(grammarFile, error)) {
        std::cerr << "加载文法失败： " << error << "\n";
        return;
    }

    std::string source = readFileToString(sourceFile);
    Lexer lexer;
    lexer.configureFromTerminals(g.terminals);

    std::vector<Token> tokens;
    if (!lexer.tokenize(source, tokens, error)) {
        std::cerr << "词法分析出错： " << error << "\n";
        return;
    }

    // --- 修改点：开始集成你的 ICG 模块 ---

    // 5. 创建你的生成器实例
    // 注意：假设你的类名为 PL0ICG，且构造函数接收 Token 向量
    // ... 词法分析得到 tokens 后 ...
    PL0ICG icg(tokens);
    std::vector<std::string> tac;

    try {
        // 6. 执行递归下降分析并生成三地址码
        // generate 方法内部应调用 program() 根规则
        if (!icg.generate(tac, error)) {
            std::cerr << "中间代码生成失败： " << error << "\n";
            return;
        }

        // 7. 输出结果到控制台
        std::cout << "生成的三地址中间代码如下：\n";
        for (const auto& line : tac) {
            std::cout << line << "\n";
        }

        // 8. 调用已有的工具函数保存到文件
        std::string outPath = dumpTacToFile(sourceFile, tac);
        std::cout << "\n已将三地址码保存到文件: " << outPath << "\n";

    }
    catch (const std::exception& e) {
        // 捕获递归下降中的语法错误（例如 expect 失败抛出的异常）
        std::cerr << "语义/语法错误: " << e.what() << "\n";
    }

   
}



int main(int argc, char **argv) {
    setupConsole();

    if (argc < 2) {
        std::cout << "用法：\n";
        std::cout << "  " << argv[0] << " part1 <grammar-file> <source-file>\n";
        std::cout << "  " << argv[0] << " part2 <source-file>\n";
        return 0;
    }

    std::string mode = argv[1];
    if (mode == "part1") {
        if (argc < 4) {
            std::cerr << "part1 模式需要提供 <grammar-file> 和 <source-file>\n";
            return 1;
        }
        runPart1(argv[2], argv[3]);
    }
    else if (mode == "part2") {
        if (argc < 4) {
            std::cerr << "part2 模式需要提供 <grammar-file> 和 <source-file>\n";
            return 1;
        }
        runPart2(argv[2], argv[3]);
    } else {
        std::cerr << "未知的模式: " << mode << "（应为 part1 或 part2）\n";
        return 1;
    }
    return 0;
}

