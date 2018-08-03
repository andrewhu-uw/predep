/// Copyright 2018 Andrew Hu
#ifndef EXPRAST_H
#define EXPRAST_H

#include<iostream>
#include<vector>
#include<memory>
#include<string>

#include "Type.h"

namespace dep
{
    /// Base class for all expression nodes.
    class ExprAST {
    public:
        virtual ~ExprAST() {}
    };

    class ImmediateAST : public ExprAST {
    };

    //! Expression class for floating point literals like "1.0".
    class FloatExprAST : public ImmediateAST {
        double FloatVal;
    public:
        FloatExprAST(double Val) : FloatVal(Val) {}
    };

    //! Expression class for floating point literals like "1.0".
    class IntExprAST : public ImmediateAST {
        long IntVal;
    public:
        IntExprAST(long Val) : IntVal(Val) {}
    };

    class BoolExprAST : public ImmediateAST {
        bool _BoolVal;
    public:
        BoolExprAST(bool val) : _BoolVal(val) {}
    };

    //! Expression class for referencing a variable, like "a".
    class VariableExprAST : public ExprAST {
        std::string Name;

    public:
        VariableExprAST(const std::string &Name) : Name(Name) {}
    };

    //! Expression class for a binary operator.
    class BinaryExprAST : public ExprAST {
        char Op;
        std::unique_ptr<ExprAST> LHS, RHS;

    public:
        BinaryExprAST(char op, std::unique_ptr<ExprAST> LHS,
            std::unique_ptr<ExprAST> RHS)
            : Op(op), LHS(std::move(LHS)), RHS(std::move(RHS)) {}
    };

    //! Expression class for function calls.
    class CallExprAST : public ExprAST {
        std::string Callee;
        std::vector<std::unique_ptr<ExprAST>> Args;

    public:
        CallExprAST(const std::string &Callee,
            std::vector<std::unique_ptr<ExprAST>> Args)
            : Callee(Callee), Args(std::move(Args)) {}
    };

    class IfExprAST : public ExprAST {
        std::unique_ptr<ExprAST> Cond, Then, Else;
    public:
        IfExprAST(std::unique_ptr<ExprAST> Cond, std::unique_ptr<ExprAST> Then,
            std::unique_ptr<ExprAST> Else)
            : Cond(std::move(Cond)), Then(std::move(Then)), Else(std::move(Else)) {}
    };

    /*!  This class represents the "prototype" for a function,
        which captures its name, and its argument names (thus implicitly the number
        of arguments the function takes).*/
    class PrototypeAST {
        std::string Name;
        std::vector<std::string> Types;
        std::vector<std::string> Args;

    public:
        PrototypeAST(const std::string &name, std::vector<std::string> Types, std::vector<std::string> Args)
            : Name(name), Types(std::move(Types)), Args(std::move(Args)) {}

        const std::string &getName() const { return Name; }
    };

    //! This class represents a function definition itself.
    class FunctionAST {
        std::unique_ptr<PrototypeAST> Proto;
        std::unique_ptr<ExprAST> Body;

    public:
        FunctionAST(std::unique_ptr<PrototypeAST> Proto,
            std::unique_ptr<ExprAST> Body)
            : Proto(std::move(Proto)), Body(std::move(Body)) {}
    };
}

#endif