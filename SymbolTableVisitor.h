/// Copyright 2018 Andrew Hu
#ifndef SYMBOL_TABLE_VISITOR_H
#define SYMBOL_TABLE_VISITOR_H

#include <map>
#include <string>
#include <memory>

#include "Visitor.h"
#include "SymbolTable.h"

namespace dep {
    SymbolTable table;
    class SymbolTableVisitor : public Visitor {
    public:
        void visit(DefineAST& ast) {}
        void visit(InitAST& ast) {}
        void visit(BlockAST& ast) {}
        void visit(AssignAST& ast) {}
    };
}

#endif