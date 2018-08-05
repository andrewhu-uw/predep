/// Copyright 2018 Andrew Hu
#ifndef SYMBOL_TABLE_BUILDER_H
#define SYMBOL_TABLE_BUILDER_H

#include <map>
#include <string>
#include <memory>

#include "SymbolTable.h"

namespace dep {

    class DefineAST;
    class InitAST;
    class BlockAST;
    class AssignAST;

    class SymbolTableBuilder {
    public:
        void visit(DefineAST& ast) {}
        void visit(InitAST& ast) {}
        void visit(BlockAST& ast) {}
        void visit(AssignAST& ast) {}
    };
}

#endif