#ifndef STATEMENT_AST_H
#define STATEMENT_AST_H

#include<memory>
#include<map>

#include"ExprAST.h"

namespace dep {
    class StatementAST {
    public:
        virtual ~StatementAST() {}
    };

    class AssignAST {
        std::unique_ptr<VariableExprAST> dest;
        std::unique_ptr<ExprAST> src;
    public:
        AssignAST(std::unique_ptr<VariableExprAST> d,
            std::unique_ptr<ExprAST> s) : dest(std::move(d)), src(std::move(s)) {}
    };

    class SingleIfAST {
        std::unique_ptr<ExprAST> cond;
        std::unique_ptr<BlockAST> thenBlock;
    public:
        SingleIfAST(std::unique_ptr<ExprAST> Cond, std::unique_ptr<BlockAST> ThenBlock) :
            cond(std::move(Cond)), thenBlock(std::move(ThenBlock)) {}
    };

    class IfAST {
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

    class WhileAST {
        std::unique_ptr<ExprAST> cond;
        std::unique_ptr<BlockAST> body;
    public:
        WhileAST(std::unique_ptr<ExprAST> Cond, std::unique_ptr<BlockAST> Body) :
            cond(std::move(Cond)), body(std::move(Body)) {}
    };

    class BlockAST {
        std::vector<std::unique_ptr<StatementAST>> statements;
    public:
        BlockAST(std::vector<std::unique_ptr<StatementAST>> s) : statements(s) {}
    };
}
#endif // !STATEMENT_AST_H
