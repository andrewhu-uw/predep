/// Copyright 2018 Andrew Hu

#include <map>
#include <string>
#include <memory>

#include "StatementAST.h"
#include "SymbolTable.h"
#include "SymbolTableBuilder.h"

namespace dep {
    SymbolTable SymbolTableBuilder::build(StatementAST one_line) {
        return SymbolTable();
    }
}