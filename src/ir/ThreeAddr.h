#pragma once

#include <string>
#include <vector>

// 三地址码操作类型（可扩展）
enum class OpCode {
    Assign,   // x = y
    Add,      // x = y + z
    Sub,      // x = y - z
    CmpGt,    // x = (y > z)
    Jmp,      // jmp L
    JmpIf     // jmpif cond L
};

struct ThreeAddrCode {
    OpCode op;
    std::string result;
    std::string arg1;
    std::string arg2;
};

using IRList = std::vector<ThreeAddrCode>;



