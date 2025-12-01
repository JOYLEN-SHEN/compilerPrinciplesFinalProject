#include "Token.h"
#include "../Common.h"
#include "../Config.h"

#include <vector>
#include <string>

using std::string;
using std::vector;

// TODO: 可以增加一些辅助函数，例如判断是否为标识符首字符 / 组成字符等

// 练习版词法分析入口：将源代码拆分为 Token 序列
vector<Token> runLexer(const string &source, vector<string> &errors) {
    // TODO: 实现一个手写扫描器：
    // 1. 跳过空白字符，正确维护行号
    // 2. 识别标识符 / 关键字（根据 g_lexConfig.keywords 判断是否为关键字）
    // 3. 识别整数常量
    // 4. 识别运算符和界符（:= + - > ; ( )）
    // 5. 对非法字符将错误信息写入 errors，并生成 Unknown token
    (void)source;
    (void)errors;
    return {};
}



