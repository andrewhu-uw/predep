/// Copyright 2018 Andrew Hu
#include<iostream>
#include<vector>
#include<memory>
#include<string>

#include "llvm/IR/Value.h"

#include "Type.h"
#include "ExprAST.h"

using llvm::Value;

namespace dep {
    Value* FloatExprAST::codegen() {
        return nullptr;
    }

    Value* IntExprAST::codegen() {
        return nullptr;
    }

    Value* BoolExprAST::codegen() {
        return nullptr;
    }

    Value* VariableExprAST::codegen() {
        return nullptr;
    }

    Value* TupleExprAST::codegen() {
        return nullptr;
    }

    Value* BinaryExprAST::codegen() {
        return nullptr;
    }

    Value* CallExprAST::codegen() {
        return nullptr;
    }

    Value* IfExprAST::codegen() {
        return nullptr;
    }
}