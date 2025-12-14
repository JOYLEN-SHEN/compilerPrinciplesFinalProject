# 编译原理课程大作业：Compiler-Compiler 开发文档

## 项目概述

本项目是一个用 C++ 实现的**简易编译器-编译器**（Compiler-Compiler），要求实现以下两个核心功能：

1. **第 1 部分**：根据输入的文法描述文件，自动生成该文法的词法分析器和 LL(1) 语法分析器
2. **第 2 部分**：对固定的 PL/0 子集语言进行语法分析，并在分析过程中生成三地址码形式的中间代码

## 项目结构

```
compilerPrinciplesFinalProject/
├── src/                    # 源代码目录
│   ├── main.cpp           # 主程序（已实现，负责整合各部分）
│   ├── grammar/           # 文法相关（成员1 + 成员2）
│   │   ├── grammar.h      # 文法数据结构定义
│   │   └── grammar.cpp    # 文法加载、FIRST/FOLLOW 计算、LL(1) 表构造
│   ├── lexer/             # 词法分析器（成员1）
│   │   ├── lexer.h
│   │   └── lexer.cpp
│   ├── parser/            # 语法分析器（成员2）
│   │   ├── ll1_parser.h
│   │   └── ll1_parser.cpp
│   └── pl0/               # PL/0 中间代码生成（成员3）
│       ├── pl0_icg.h
│       └── pl0_icg.cpp
├── grammar_files/         # 文法描述文件（测试用例）
│   ├── expr_grammar.txt   # 表达式文法示例
│   ├── if_grammar.txt     # if 语句文法示例
│   └── pl0_grammar.txt    # PL/0 文法示例
├── tests/                 # 测试用例
│   ├── expr_ok_1.txt      # 表达式测试用例（正确）
│   ├── expr_ok_2.txt
│   ├── expr_err_1.txt     # 表达式测试用例（错误）
│   ├── if_ok_1.txt        # if 语句测试用例（正确）
│   ├── if_ok_2.txt
│   ├── if_err_1.txt       # if 语句测试用例（错误）
│   ├── pl0_sample1.txt    # PL/0 测试用例
│   ├── pl0_sample2.txt
│   └── pl0_err1.txt       # PL/0 错误测试用例
├── output/                # 输出目录（程序自动创建）
└── README.md             # 本文件

```

## 三人分工

### 成员1：词法分析器 + 文法加载基础 余伟强

**负责模块：**
- `src/lexer/lexer.h` 和 `src/lexer/lexer.cpp`（完整实现）
- `src/grammar/grammar.h` 和 `src/grammar/grammar.cpp` 中的以下函数：
  - `trim()` - 字符串处理工具函数
  - `splitSymbols()` - 产生式右部符号拆分
  - `Grammar::loadFromFile()` - 文法文件加载和解析

**主要任务：**
1. **词法分析器（Lexer）**
   - 实现 `configureFromTerminals()`：根据文法中的终结符集合自动配置词法分析器
   - 实现 `tokenize()`：对源程序进行词法分析，识别标识符、数字、关键字、运算符等
   - 实现 `createPL0Lexer()`：创建固定的 PL/0 词法分析器
   - 实现辅助函数：`isIdentifierStart()`, `isIdentifierChar()`

2. **文法加载基础**
   - 实现 `trim()`：去除字符串首尾空白
   - 实现 `splitSymbols()`：解析产生式右部，支持带引号的终结符（如 `"if"`, `"+"`）
   - 实现 `Grammar::loadFromFile()`：从文件读取文法，解析产生式，识别终结符和非终结符

**参考算法：**
- 词法分析：状态机方法，最长匹配原则
- 文法文件格式：简化 BNF，支持 `%Start` 指令和 `->` 产生式

**测试用例：**
- 使用 `grammar_files/expr_grammar.txt` 和 `tests/expr_ok_1.txt` 测试词法分析
- 使用 `grammar_files/pl0_grammar.txt` 测试 PL/0 词法分析

---

### 成员2：语法分析器 + FIRST/FOLLOW 集合计算 沈仲尧

**负责模块：**
- `src/parser/ll1_parser.h` 和 `src/parser/ll1_parser.cpp`（完整实现）
- `src/grammar/grammar.cpp` 中的以下函数：
  - `Grammar::computeFirstSets()` - FIRST 集合计算
  - `Grammar::computeFollowSets()` - FOLLOW 集合计算
  - `Grammar::buildLL1ParseTable()` - LL(1) 预测分析表构造

**主要任务：**
1. **FIRST 集合计算**
   - 实现迭代算法计算所有符号的 FIRST 集合
   - 处理 epsilon 产生式
   - 处理产生式右部符号序列的 FIRST 集合

2. **FOLLOW 集合计算**
   - 实现迭代算法计算所有非终结符的 FOLLOW 集合
   - 处理产生式右部末尾符号的情况
   - 处理可推导出 epsilon 的情况

3. **LL(1) 预测分析表构造**
   - 根据 FIRST/FOLLOW 集合构造预测分析表
   - 检测 LL(1) 冲突（同一表项有多个产生式）

4. **LL(1) 表驱动语法分析**
   - 实现栈驱动的预测分析算法
   - 根据分析表选择产生式
   - 记录产生式使用序列
   - 错误处理和错误信息生成

**参考算法：**
- FIRST 集合：迭代不动点算法
- FOLLOW 集合：迭代不动点算法
- LL(1) 分析：表驱动预测分析

**测试用例：**
- 使用 `grammar_files/expr_grammar.txt` 和 `tests/expr_ok_1.txt` 测试语法分析
- 使用 `grammar_files/if_grammar.txt` 和 `tests/if_ok_1.txt` 测试不同文法
- 使用错误测试用例验证错误处理

---

### 成员3：PL/0 中间代码生成 王思源

**负责模块：**
- `src/pl0/pl0_icg.h` 和 `src/pl0/pl0_icg.cpp`（完整实现）

**主要任务：**
1. **递归下降语法分析器**
   - 实现 PL/0 子集的递归下降分析器
   - 实现所有语法规则的解析函数

2. **语法制导翻译**
   - 在语法分析过程中同步生成三地址码
   - 维护符号表（常量、变量）
   - 生成临时变量和标签

3. **三地址码生成**
   - 表达式翻译：生成算术运算的三地址码
   - 条件语句翻译：生成条件跳转代码
   - 循环语句翻译：生成循环控制代码
   - 赋值语句翻译：生成赋值代码

**PL/0 子集文法：**
```text
program   ::= block "."
block     ::= [ "const" ident "=" number { "," ident "=" number } ";" ]
              [ "var" ident { "," ident } ";" ]
              "begin" statement { ";" statement } "end"

statement ::= ident ":=" expression
            | "begin" statement { ";" statement } "end"
            | "if" condition "then" statement
            | "while" condition "do" statement

condition ::= "odd" expression
            | expression relop expression

relop     ::= "=" | "#" | "<" | "<=" | ">" | ">="

expression ::= ["+"|"-"] term { ("+"|"-") term }
term       ::= factor { ("*"|"/") factor }
factor     ::= ident | number | "(" expression ")"
```

**三地址码格式：**
- 赋值：`t1 = 2`
- 算术运算：`t2 = a + t1`
- 关系运算：`t3 = x < 10`
- 条件跳转：`ifFalse t3 goto L1`
- 无条件跳转：`goto L1`
- 标签：`L1:`

**参考算法：**
- 递归下降分析
- 语法制导翻译（Syntax-directed Translation）
- 属性文法

**测试用例：**
- 使用 `tests/pl0_sample1.txt` 和 `tests/pl0_sample2.txt` 测试
- 使用 `tests/pl0_err1.txt` 测试错误处理

---

## 编译和运行

### 编译

在项目根目录下，使用 `g++`（需要支持 C++17）编译：

**Windows PowerShell：**
```powershell
g++ -std=c++17 -O2 -o compiler_gen.exe src/main.cpp src/grammar/grammar.cpp src/lexer/lexer.cpp src/parser/ll1_parser.cpp src/pl0/pl0_icg.cpp
```

**Linux / WSL：**
```bash
g++ -std=c++17 -O2 -o compiler_gen src/main.cpp src/grammar/grammar.cpp src/lexer/lexer.cpp src/parser/ll1_parser.cpp src/pl0/pl0_icg.cpp
```

### 运行

**第 1 部分测试（自动生成词法/语法分析器）：**
```powershell
.\compiler_gen.exe part1 grammar_files/expr_grammar.txt tests/expr_ok_1.txt
.\compiler_gen.exe part1 grammar_files/if_grammar.txt tests/if_ok_1.txt
.\compiler_gen.exe part1 grammar_files/pl0_grammar.txt tests/pl0_sample1.txt
```

**第 2 部分测试（PL/0 三地址码生成）：**
```powershell
.\compiler_gen.exe part2 grammar_files/pl0_grammar.txt tests/pl0_sample1.txt
.\compiler_gen.exe part2 tests/pl0_sample2.txt
```

**错误测试：**
```powershell
.\compiler_gen.exe part1 grammar_files/expr_grammar.txt tests/expr_err_1.txt
.\compiler_gen.exe part2 tests/pl0_err1.txt
```

## 开发指导

### 代码风格

1. **命名规范**：使用驼峰命名法（类名首字母大写，函数名首字母小写）
2. **注释**：关键函数和算法需要添加注释说明
3. **错误处理**：所有函数都应该有适当的错误处理和错误信息输出
4. **编码**：使用 UTF-8 编码，中文注释和错误信息使用中文

### 开发流程

1. **理解需求**：仔细阅读本文档和代码中的 TODO 注释
2. **设计算法**：参考编译原理教材中的经典算法
3. **实现代码**：按照 TODO 注释逐步实现
4. **单元测试**：先测试自己负责的部分
5. **集成测试**：与团队成员代码集成后测试
6. **调试优化**：修复 bug，优化代码

### 调试技巧

1. **使用 `debugPrint()`**：在 `Grammar` 类中实现调试输出，打印 FIRST/FOLLOW 集合和分析表
2. **逐步测试**：先实现基础功能，再逐步完善
3. **错误信息**：确保错误信息包含行号和列号，便于定位问题
4. **测试用例**：使用提供的测试用例验证功能

### 常见问题

1. **LL(1) 冲突**：如果文法不是 LL(1) 的，需要修改文法或使用其他分析方法
2. **编码问题**：Windows 下注意 UTF-8 编码，终端输出可能需要设置代码页
3. **文件路径**：注意相对路径和绝对路径的区别
4. **内存管理**：C++ 中注意避免内存泄漏


