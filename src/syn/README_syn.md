### syn 模块完成说明（2254281沈仲尧）

#### 1. 模块职责概览

- **语法树结构定义（`SyntaxTree.h`）**
  - 定义语法树节点类型 `NodeType`，涵盖：
    - `Program`：整个程序根节点
    - `StmtList`：语句序列
    - `AssignStmt`：赋值语句
    - `IfStmt`：if 语句
    - `BlockStmt`：`begin ... end` 语句块
    - `Expr`：加减表达式结点
    - `RelExpr`：关系表达式结点（目前支持 `>`）
    - `Term`：标识符或数字等基本项
  - 定义 `SyntaxNode` 结构：
    - `NodeType type`：节点类型
    - `Token token`：与该节点关联的关键词法单元（如标识符、运算符）
    - `std::vector<SyntaxNode*> children`：子节点
  - 提供递归释放接口 `freeSyntaxTree(SyntaxNode *node)`，负责释放整棵语法树，供主程序和后端模块安全调用。

- **递归下降语法分析器（`Parser.cpp`）**
  - 对外暴露的唯一接口：
    - `SyntaxNode* runParser(const std::vector<Token> &tokens, std::vector<std::string> &errors);`
  - 实现了一个 LL(1) 风格的递归下降 Parser，按照 README 中给定的文法规则构建语法树，并在出错时向 `errors` 中追加带行号的错误信息。

#### 2. 已实现的文法与对应函数

语法规则和对应的解析函数如下（函数均为 `Parser` 类的私有成员）：

- **程序入口**
  - 文法：`<program> -> <stmt_list> EOF`
  - 实现：`parseProgram()`  
    - 构造 `Program` 根节点，其唯一子节点为整段程序的 `StmtList`。
    - 解析结束后调用 `expect(TokenType::EndOfFile, "程序末尾应为 EOF")` 确保 token 流被完全消费。

- **语句序列**
  - 文法：`<stmt_list> -> <stmt> { ; <stmt> } [;]`
  - 实现：`parseStmtList()`  
    - 至少解析一个 `stmt`，后续在 `while` 循环中处理若干 `; stmt`。
    - 对于尾部分号 `[;]` 的处理：如果读取到分号后，下一个 token 直接是 `end` 或 `EOF`，则认为这是可选的尾分号，不再继续解析新的语句。

- **语句分派**
  - 文法：`<stmt> -> <assign> | <if> | <block>`
  - 实现：`parseStmt()`  
    - 根据当前 `peek().type` 分派到 `parseAssign / parseIf / parseBlock`。
    - 如果是其他 token（无法识别的语句起始），会：
      - 生成错误信息：`"第 <line> 行：语法错误，无法识别的语句"`；
      - 丢弃当前 token 以进行简单同步恢复；
      - 返回一个空的 `StmtList` 节点，避免崩溃。

- **赋值语句**
  - 文法：`<assign> -> id := <expr>`
  - 实现：`parseAssign()`  
    - 首先要求当前 token 为 `Identifier`，否则报错“赋值语句应以标识符开头”，并进行简单恢复。
    - 然后通过 `expect(TokenType::Assign, "赋值语句缺少 ':='")` 检查 `:=`。
    - 语法树结构：
      - 当前节点：`NodeType::AssignStmt`，`token` 中保存左值标识符 token；
      - `children[0]`：赋值右侧的表达式节点 `Expr` 或 `RelExpr`。
    - 这样后续中间代码生成只需访问 `node->token.lexeme` 即可拿到左值变量名。

- **if 语句**
  - 文法：`<if> -> if <expr> then <stmt>`
  - 实现：`parseIf()`  
    - 期望顺序：`KeywordIf` → 条件表达式 `expr` → `KeywordThen` → 语句体 `stmt`。
    - 语法树结构：
      - 当前节点：`NodeType::IfStmt`，`token` 为 `if` 对应的 token；
      - `children[0]`：条件表达式节点（`Expr` 或 `RelExpr`）；
      - `children[1]`：then 后的语句节点（可以是单条赋值 / if / block）。

- **语句块**
  - 文法：`<block> -> begin <stmt_list> end`
  - 实现：`parseBlock()`  
    - 期望 `KeywordBegin`，然后递归调用 `parseStmtList()`，最后期望 `KeywordEnd`。
    - 语法树结构：
      - 当前节点：`NodeType::BlockStmt`；
      - `children[0]`：对应块内部的 `StmtList`。

- **表达式与关系表达式**
  - 文法：
    - `<expr>     -> <add_expr> [ '>' <add_expr> ]`
    - `<add_expr> -> <term> { (+|-) <term> }`
    - `<term>     -> id | number`
  - 实现：
    - `parseExpr()`：
      - 先解析一个加减表达式 `add_expr`；
      - 若后面跟 `>`，则构造 `RelExpr` 节点，`token` 为 `>`，左右子节点分别是左右两个 `add_expr`。
    - `parseAddExpr()`：
      - 构造左结合的运算树：((t1 + t2) - t3) ...
      - 对每个 `+/-` 运算符建立一个 `NodeType::Expr` 节点，`token` 为运算符，`children[0]` 为左操作数，`children[1]` 为右操作数。
    - `parseTerm()`：
      - 对 `Identifier` / `Number` 生成 `NodeType::Term` 叶子节点。
      - 遇到其他 token 时，报错“表达式缺少操作数”，丢弃当前 token 并返回一个占位 `Term`，避免死循环。

#### 3. 错误处理与 `errors` 向量约定

- `Parser` 内部统一使用 `reportError(const std::string &msg)` 生成错误消息：
  - 统一格式：`"第 <line> 行：语法错误，" + msg`。
  - 行号来自当前未消费的 `peek()` token。
- `expect(TokenType expected, const std::string &msg)` 在未匹配到期望 token 时会调用 `reportError`，但**不会强行移动指针**，以减少级联错误；部分场景由上层函数适度消费 token 做同步。
- 错误信息全部追加到 `runParser` 传入的 `errors` 向量中：
  - `Main.cpp` 中会根据 `errors` 中是否含有“语法错误”来判断是否终止编译。

#### 4. 与其他模块的对接方式

- **与词法分析模块 (`Lexer.cpp`) 的对接**
  - `runParser` 的输入是 `runLexer` 产生的 `std::vector<Token>`。
  - 词法模块需保证：
    - 按 README 要求使用 `TokenType` 枚举（Identifier/Number/KeywordIf/.../Assign/.../EndOfFile）。
    - 在 token 流末尾追加一个 `TokenType::EndOfFile` 的 token。
  - Parser **不依赖**任何词法内部细节，只依赖 `Token` 的 `type / lexeme / line` 三个字段。

- **与中间代码生成模块 (`CodeGen.cpp`) 的对接**
  - `runCodeGen(SyntaxNode *root)` 的入参 `root`：
    - 必须是 `NodeType::Program` 根节点（由 `runParser` 保证）。
    - 根节点的 `children[0]` 是整个程序的 `StmtList`。
  - 建议 C 负责同学在生成 IR 时按以下方式遍历：
    - **程序级别**
      - `Program` → `StmtList` → 依次遍历 `children` 中的各个语句节点。
    - **赋值语句**
      - `AssignStmt`：
        - 左值变量名：`node->token.lexeme`
        - 右侧表达式：`node->children[0]`（可能是 `Expr` / `RelExpr` / `Term`）
    - **if 语句**
      - `IfStmt`：
        - 条件：`children[0]`
        - 语句体：`children[1]`
    - **语句块**
      - `BlockStmt`：
        - `children[0]` 为内部 `StmtList`，递归遍历即可。
    - **表达式树**
      - `RelExpr`：`children[0]` 和 `children[1]` 分别是左右子表达式，`token.lexeme` 为关系运算符（当前为 `>`）。
      - `Expr`：作为加减运算结点，`token.lexeme` 为 `+` 或 `-`，`children[0]` / `children[1]` 为左右操作数。
      - `Term`：叶子结点，`token` 为标识符或数字字面量。

#### 5. 使用方法

- **主流程调用方式（已在 `Main.cpp` 中写好）**
  - 先通过 `runLexer` 得到 `tokens` 和 `errors`；
  - 若没有词法错误，则调用：
    - `SyntaxNode *root = runParser(tokens, errors);`
  - 若 `errors` 中包含“语法错误”字段，则主程序会打印所有错误并退出；
  - 否则，将 `root` 交给 `runCodeGen(root)` 生成三地址码。

- **内存管理约定**
  - 所有语法树节点由 `Parser` 使用 `new` 创建；
  - 语法树的释放统一由调用方负责，使用：
    - `freeSyntaxTree(root);`
  - `runParser` 本身不自动释放树，方便后端多次遍历。

#### 6. 后续可扩展方向

- 在 `NodeType` 中扩展更多语句 / 表达式类型（如 `WhileStmt`、逻辑运算等），并在 `Parser` 中增加对应的 `parseWhile` 等函数。
- 支持更多关系运算符（`<`, `<=`, `>=`, `==`, `!=`），为 `RelExpr` 增加相应分支。
- 在错误恢复策略上可以进一步增强，例如：基于同步符号集合跳过 token，减少级联报错。

> 总结：`syn` 模块目前已经完成了课程要求内的语法树结构与递归下降 Parser，实现了与词法分析和中间代码生成模块之间的清晰接口。负责C工作的同学只需要按照本说明中的树结构约定进行遍历，就可以顺利在 `CodeGen.cpp` 中生成三地址码；负责A工作的同学只需保证 token 序列和 `EOF` 的正确性，即可与本模块无缝对接。


