### 一、目录结构

```text
compilerPrinciplesFinalProject/
  ├── src/
  │   ├── Common.h
  │   ├── Config.h
  │   ├── Config.cpp
  │   ├── Main.cpp
  │   ├── lex/
  │   │   ├── Token.h
  │   │   └── Lexer.cpp
  │   ├── syn/
  │   │   ├── SyntaxTree.h
  │   │   └── Parser.cpp
  │   └── ir/
  │       ├── ThreeAddr.h
  │       └── CodeGen.cpp
  └── test/
      ├── pl0_lex_rules.txt
      ├── pl0_syn_rules.txt
      ├── README.md
      ├── test_ok.txt
      ├── test_ok_nested.txt
      ├── test_lex_err.txt
      ├── test_syn_err.txt
      ├── test_syn_missing_then.txt
      └── test_syn_missing_end.txt
```

### 二、各源文件的预期内容与任务

#### 1. `src/Common.h`

- **作用**：提供通用的文件读取与字符串工具函数。
- 建议包含内容：
  - `readFileAsString(path)`：读取整个文件为字符串。
  - `readFileLines(path)`：按行读取文件为 `std::vector<std::string>`。
  - `trim(s)`：去除字符串首尾空白。

#### 2. `src/Config.h` / `src/Config.cpp`

- **作用**：封装“基于规则文件的配置加载”，体现 Compiler-Compiler 思想。
- 结构：
  - `struct LexRuleConfig`：至少包含关键字集合 `std::unordered_set<std::string> keywords;`
  - `struct SynRuleConfig`：存储语法产生式字符串 `std::vector<std::string> productions;`
  - 全局对象：`extern LexRuleConfig g_lexConfig; extern SynRuleConfig g_synConfig;`
  - 函数：
    - `bool loadLexRules(const std::string&, LexRuleConfig&, std::vector<std::string>&);`
    - `bool loadSynRules(const std::string&, SynRuleConfig&, std::vector<std::string>&);`
- 练习任务：
  - 实现从 `test/pl0_lex_rules.txt` 读取关键字；
  - 实现从 `test/pl0_syn_rules.txt` 读取文法产生式文本；
  - 对错误格式给出合理的错误消息。

#### 3. `src/lex/Token.h`

- **作用**：定义词法单元类型和结构。
- 建议包含：
  - `enum class TokenType { Identifier, Number, KeywordIf, ... , EndOfFile, Unknown };`
  - `struct Token { TokenType type; std::string lexeme; int line; };`
  - `std::string tokenTypeToString(TokenType t);` 的声明。
- 练习任务：
  - 后续在 `Lexer.cpp` 中使用这些类型构建 Token 流。

#### 4. `src/lex/Lexer.cpp`

- **作用**：实现 **词法分析器**（Scanner），将源程序拆分为 Token 流。
- 接口：
  - `std::vector<Token> runLexer(const std::string &source, std::vector<std::string> &errors);`
- 建议实现要点：
  - 跳过空白与换行，对行号进行记录；
  - 识别标识符 / 关键字（关键字应根据 `g_lexConfig.keywords` 判定）；
  - 识别整数常量、运算符（`:= + - > ; ( )` 等）；
  - 对非法字符，向 `errors` 推入类似 `“第 x 行：非法字符 'c'”` 的信息。

#### 5. `src/syn/SyntaxTree.h`

- **作用**：定义语法树节点结构。
- 建议结构：
  - `enum class NodeType { Program, StmtList, AssignStmt, IfStmt, BlockStmt, Expr, RelExpr, Term };`
  - `struct SyntaxNode { NodeType type; Token token; std::vector<SyntaxNode*> children; ... };`
  - 递归释放函数 `freeSyntaxTree(SyntaxNode *node);`

#### 6. `src/syn/Parser.cpp`

- **作用**：实现 LL(1) 风格的递归下降 **语法分析器**。
- 已保留接口：
  - `SyntaxNode* runParser(const std::vector<Token> &tokens, std::vector<std::string> &errors);`
- 推荐语法规则（已在 `test/pl0_syn_rules.txt` 中体现）：
  - `<program>   -> <stmt_list>`
  - `<stmt_list> -> <stmt> { ; <stmt> } [;]`
  - `<stmt>      -> <assign> | <if> | <block>`
  - `<assign>    -> id := <expr>`
  - `<if>        -> if <expr> then <stmt>`
  - `<block>     -> begin <stmt_list> end`
  - `<expr>      -> <add_expr> [ '>' <add_expr> ]`
  - `<add_expr>  -> <term> { (+|-) <term> }`
  - `<term>      -> id | number`
- 目标：
  - 按上述文法实现 `parseProgram/parseStmtList/parseStmt/...`；
  - 对缺失 token（如缺少 `then/end/;`）生成带行号的语法错误信息。

#### 7. `src/ir/ThreeAddr.h`

- **作用**：定义三地址码（中间代码）的基本数据结构。
- 建议结构：
  - `enum class OpCode { Assign, Add, Sub, CmpGt, Jmp, JmpIf };`
  - `struct ThreeAddrCode { OpCode op; std::string result, arg1, arg2; };`
  - `using IRList = std::vector<ThreeAddrCode>;`

#### 8. `src/ir/CodeGen.cpp`

- **作用**：根据语法树生成三地址码，并提供打印函数。
- 已保留接口：
  - `IRList runCodeGen(SyntaxNode *root);`
  - `void printIR(const IRList &ir, std::ostream &os);`
- 推荐功能：
  - 支持赋值语句：`x := expr`；
  - 支持表达式：`+` / `-`；
  - 支持关系表达式：`>`，生成 `CmpGt`；
  - 支持 `if <expr> then <stmt>`，用 `JmpIf` + `Jmp` 近似表示控制流。

#### 9. `src/Main.cpp`

- **作用**：程序入口，串联整个编译流程。
- 已保留职责：
  - 从命令行读取三个参数：`<lex_rules> <syn_rules> <source_file>`；
  - 调用 `loadLexRules` / `loadSynRules` 构造配置；
  - 读取源程序文本；
  - 依次调用 `runLexer` / `runParser` / `runCodeGen`；
  - 若前两阶段出现错误，打印错误并退出；
  - 否则将 IR 使用 `printIR` 输出到 `ir.txt`。

### 三、测试用例与运行方式

`test/` 目录中已复制了完整项目的测试用例与说明（见其中 `README.md`）：

- **词法正确 + 语法正确**：
  - `test_ok.txt`：最简单的 `begin ... if ... end` 程序，期望生成合理的三地址码。
  - `test_ok_nested.txt`：包含多条语句和嵌套块。
- **词法错误**：
  - `test_lex_err.txt`：包含非法字符 `#`。
- **语法错误**：
  - `test_syn_err.txt`：缺少分号；
  - `test_syn_missing_then.txt`：`if` 后缺少 `then`；
  - `test_syn_missing_end.txt`：`begin` 和 `end` 不匹配。

由于我使用的是vscode + mingw G++编译器，在终端使用以下命令测试。

```bash
g++ src/Config.cpp src/lex/Lexer.cpp src/syn/Parser.cpp src/ir/CodeGen.cpp src/Main.cpp -o compiler.exe
```

运行exe并测试不同的终端：

```bash
./compiler.exe test/pl0_lex_rules.txt test/pl0_syn_rules.txt test/test_ok.txt
```

- 对合法程序，终端应提示“生成三地址码到 ir.txt 完成”，并在当前目录生成 `ir.txt`；
- 对错误程序，应在标准错误输出中给出清晰的词法或语法错误提示，并不生成/更新 `ir.txt`。

### 四、开发顺序

1. **先实现 Token / 词法分析器**：通过 `test_lex_err.txt` 与 `test_ok.txt` 的词法输出。
2. **再实现 Parser**：确保各种语法错误信息能覆盖测试描述中的场景。
3. **最后实现 CodeGen**：让 `test_ok.txt` 与 `test_ok_nested.txt` 生成的 IR 与预期结构一致。
4. **扩展方向**（选做）：
   - 在 `pl0_syn_rules.txt` 中增加新的语法规则（如 `while`）并扩展 Parser；
   - 为 IR 增加乘除、布尔运算等。

### 五、建议分工

- **成员 A：词法分析与公共工具**
  - 负责：`Common.h`、`Config.h/Config.cpp` 的基本实现，以及 `lex/Token.h`、`lex/Lexer.cpp` 中的词法分析逻辑。
  - 工作重点：文件读取与字符串处理、规则文件解析（构造 `g_lexConfig`）、将源程序切分为正确的 Token 流，并输出清晰的词法错误信息。

- **成员 B：语法分析与语法树**
  - 负责：`syn/SyntaxTree.h`、`syn/Parser.cpp`。
  - 工作重点：根据 `pl0_syn_rules.txt` 设计并实现递归下降 Parser，构建语法树结构，对各种语法错误（缺少 `then/end/;`、无法识别的语句等）给出带行号的报错。

- **成员 C：中间代码生成与集成测试**
  - 负责：`ir/ThreeAddr.h`、`ir/CodeGen.cpp` 以及 `Main.cpp` 中主流程（调用顺序、错误处理与 `ir.txt` 输出）。
  - 工作重点：基于语法树生成三地址码（赋值、表达式、关系运算与 `if` 控制流），设计 IR 输出格式，并组织使用 `test` 下所有用例进行集成测试和结果记录。

