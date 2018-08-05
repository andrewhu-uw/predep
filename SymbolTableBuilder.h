/// Copyright 2018 Andrew Hu
#ifndef SYMBOL_TABLE_BUILDER_H
#define SYMBOL_TABLE_BUILDER_H

#include <map>
#include <string>
#include <memory>

#include "StatementAST.h"
#include "SymbolTable.h"

namespace dep {
    class SymbolTableBuilder {
    public:
        SymbolTable build(StatementAST one_line);
    };
}

#endif