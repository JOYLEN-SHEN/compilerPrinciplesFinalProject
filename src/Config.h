#pragma once

#include <string>
#include <unordered_set>
#include <vector>

// 词法规则配置：例如关键字集合
struct LexRuleConfig {
    std::unordered_set<std::string> keywords;
};

// 语法规则配置：保存产生式文本
struct SynRuleConfig {
    std::vector<std::string> productions;
};

// 全局配置对象
extern LexRuleConfig g_lexConfig;
extern SynRuleConfig g_synConfig;

// 从文件加载词法规则
bool loadLexRules(const std::string &path, LexRuleConfig &config, std::vector<std::string> &errors);

// 从文件加载语法规则
bool loadSynRules(const std::string &path, SynRuleConfig &config, std::vector<std::string> &errors);



