#ifndef STATEMENT_AST_H
#define STATEMENT_AST_H

#include<memory>
#include<map>
#include<string>

#include "ExprAST.h"

namespace dep {
    // Forward definition necessary here
    class Visitor;
    
    class StatementAST {
    public:
        virtual ~StatementAST() {}
        virtual void accept(Visitor& b) {}
    };

    class BlockAST {
        std::vector<std::unique_ptr<StatementAST>> statements;
    public:
        BlockAST(std::vector<std::unique_ptr<StatementAST>> s) : statements(std::move(s)) {}
        /*!
            Important: function only declared here, not defined because definition requires definition
            of Visitor class which would create a circular dependency. Thanks C++...
        */
        //virtual void accept(Visitor& builder);
    };

    class AssignAST : public StatementAST {
        std::unique_ptr<VariableExprAST> dest;
        std::unique_ptr<ExprAST> src;
    public:
        AssignAST(std::unique_ptr<VariableExprAST> d,
            std::unique_ptr<ExprAST> s) : dest(std::move(d)), src(std::move(s)) {}
    };

    class DefineAST : public StatementAST {
        std::string type, name;
    public:
        DefineAST(std::string Type, std::string Name) : type(Type), name(Name) {}
    };

    class InitAST : public StatementAST {
        std::unique_ptr<DefineAST> def;
        std::unique_ptr<ExprAST> rval;
    public:
        InitAST(std::unique_ptr<DefineAST> Def, std::unique_ptr<ExprAST> Rval) :
            def(std::move(Def)), rval(std::move(Rval)) {}
    };

    class CallStatementAST : public StatementAST {
        std::unique_ptr<CallExprAST> call;
    public:
        CallStatementAST(std::unique_ptr<CallExprAST> Call) : call(std::move(Call)) {}
    };

    class SingleIfAST : public StatementAST {
        std::unique_ptr<ExprAST> cond;
        std::unique_ptr<BlockAST> thenBlock;
    public:
        SingleIfAST(std::unique_ptr<ExprAST> Cond, std::unique_ptr<BlockAST> ThenBlock) :
            cond(std::move(Cond)), thenBlock(std::move(ThenBlock)) {}
    };

    class IfAST : public StatementAST {
        std::unique_ptr<ExprAST> cond;
        std::unique_ptr<BlockAST> thenBlock;
        std::vector<SingleIfAST> elseIfVec;
        std::unique_ptr<BlockAST> elseBlock;
    public:
        IfAST(std::unique_ptr<ExprAST> Cond, std::unique_ptr<BlockAST> ThenBlock,
            std::vector<SingleIfAST> ElseIfVec, std::unique_ptr<BlockAST> ElseBlock) :
                cond(std::move(Cond)), thenBlock(std::move(ThenBlock)),
                elseIfVec(std::move(ElseIfVec)), elseBlock(std::move(ElseBlock)) {}
    };

    class WhileAST : public StatementAST {
        std::unique_ptr<ExprAST> cond;
        std::unique_ptr<BlockAST> body;
    public:
        WhileAST(std::unique_ptr<ExprAST> Cond, std::unique_ptr<BlockAST> Body) :
            cond(std::move(Cond)), body(std::move(Body)) {}
    };
}  // end namespace dep
#endif // !STATEMENT_AST_H
