#include "Config.h"
#include "Common.h"


LexRuleConfig g_lexConfig;
SynRuleConfig g_synConfig;

bool loadLexRules(const std::string &path, LexRuleConfig &config, std::vector<std::string> &errors) {
    // TODO:
    // 1. 使用 readFileLines 读入规则文件
    // 2. 跳过空行和以 // 开头的注释
    // 3. 解析形如 "keyword if" 的行，把关键字放入 config.keywords
    // 4. 发现错误格式时向 errors 追加说明
    (void)path;
    (void)config;
    (void)errors;
    return false;
}

bool loadSynRules(const std::string &path, SynRuleConfig &config, std::vector<std::string> &errors) {
    // TODO:
    // 1. 使用 readFileLines 读入文法文件
    // 2. 跳过空行和注释，将每一条产生式字符串保存到 config.productions
    // 3. 若没有有效产生式，向 errors 报告
    (void)path;
    (void)config;
    (void)errors;
    return false;
}



