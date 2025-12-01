#include "ThreeAddr.h"
#include "../syn/SyntaxTree.h"

#include <vector>
#include <string>
#include <ostream>

// TODO: 中间代码生成与打印

// 根据语法树生成三地址码
IRList runCodeGen(SyntaxNode *root) {
    // TODO:
    // 1. 深度优先遍历语法树
    // 2. 对赋值语句、if 语句、表达式等生成对应的 ThreeAddrCode 序列
    // 3. 使用临时变量 t1, t2, ... 表示中间结果
    (void)root;
    return {};
}

// 打印三地址码到输出流
void printIR(const IRList &ir, std::ostream &os) {
    // TODO: 按行打印形如：
    //   t1 = 1 + 2
    //   a = t1
    //   jmpif t1 L1
    (void)ir;
    (void)os;
}



