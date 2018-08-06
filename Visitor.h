/// Copyright 2018 Andrew Hu
#ifndef SYMBOL_TABLE_BUILDER_H
#define SYMBOL_TABLE_BUILDER_H

#include <map>
#include <string>
#include <memory>

#include "StatementAST.h"

namespace dep {
    class Visitor {
    public:
        virtual void visit(DefineAST& ast) = 0;
        virtual void visit(InitAST& ast) = 0;
        virtual void visit(BlockAST& ast) = 0;
        virtual void visit(AssignAST& ast) = 0;
    };
}

#endif