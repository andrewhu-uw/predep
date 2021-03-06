/// Copyright 2018 Andrew Hu
#ifndef EXPRAST_H
#define EXPRAST_H

#include<iostream>
#include<vector>
#include<memory>
#include<string>

#include "llvm/IR/Value.h"

#include "Type.h"

namespace dep {
    /// Base class for all expression nodes.
    class ExprAST {
    public:
        virtual ~ExprAST() {}
        virtual llvm::Value* codegen() = 0;
    };

    class ImmediateAST : public ExprAST {};

    //! Expression class for floating point literals like "1.0".
    class FloatExprAST : public ImmediateAST {
        double FloatVal;
    public:
        FloatExprAST(double Val) : FloatVal(Val) {}
        llvm::Value* codegen();
    };

    //! Expression class for floating point literals like "1.0".
    class IntExprAST : public ImmediateAST {
        long IntVal;
    public:
        IntExprAST(long Val) : IntVal(Val) {}
        llvm::Value* codegen();
    };

    class BoolExprAST : public ImmediateAST {
        bool _BoolVal;
    public:
        BoolExprAST(bool val) : _BoolVal(val) {}
        llvm::Value* codegen();
    };

    //! Expression class for referencing a variable, like "a".
    class VariableExprAST : public ExprAST {
        std::string Name;
    public:
        VariableExprAST(const std::string &Name) : Name(Name) {}
        llvm::Value* codegen();
    };

    class TupleExprAST : public ExprAST {
        std::vector<std::unique_ptr<ExprAST>> args;
    public:
        TupleExprAST(std::vector<std::unique_ptr<ExprAST>> Args) : args(std::move(Args)) {}
        llvm::Value* codegen();
    };

    //! Expression class for a binary operator.
    class BinaryExprAST : public ExprAST {
        char Op;
        std::unique_ptr<ExprAST> LHS, RHS;
    public:
        BinaryExprAST(char op, std::unique_ptr<ExprAST> LHS,
            std::unique_ptr<ExprAST> RHS)
            : Op(op), LHS(std::move(LHS)), RHS(std::move(RHS)) {}
        llvm::Value* codegen();
    };

    //! Expression class for function calls.
    class CallExprAST : public ExprAST {
        std::string Callee;
        std::unique_ptr<TupleExprAST> Args;
    public:
        CallExprAST(const std::string &Callee,
            std::unique_ptr<TupleExprAST> Args)
            : Callee(Callee), Args(std::move(Args)) {}
        llvm::Value* codegen();
    };

    class IfExprAST : public ExprAST {
        std::unique_ptr<ExprAST> Cond, Then, Else;
    public:
        IfExprAST(std::unique_ptr<ExprAST> Cond, std::unique_ptr<ExprAST> Then,
            std::unique_ptr<ExprAST> Else)
            : Cond(std::move(Cond)), Then(std::move(Then)), Else(std::move(Else)) {}
        llvm::Value* codegen();
    };
}

#endif