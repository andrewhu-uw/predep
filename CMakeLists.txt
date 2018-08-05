cmake_minimum_required (VERSION 2.8)
set(INTERP_VERSION_MAJOR 0)
set(INTERP_VERSION_MINOR 1)

set (INTERP_VERSION "${INTERP_VERSION_MAJOR}.${INTERP_VERSION_MINOR}")
set (DEP_VS_DEFAULT_PROJECT "interp" CACHE STRING "The default project to be run by Visual Studio")

project (Predep)

# Pass CMake configurations into a real header
# (generates file in current binary directory)
configure_file (InterpConfig.h.in   InterpConfig.h)
# Install this file somewhere everybody can see
# (relative to CMAKE_INSTALL_PREFIX)
install ( FILES ${CMAKE_CURRENT_BINARY_DIR}/InterpConfig.h
          DESTINATION include/interp)

# Create the Parse library
add_library(Parser Parser.cpp Lexer.cpp ParserTerminal.cpp ExprAST.cpp 
        StatementAST.cpp StatementAST.h FunctionAST.h ParserStatement.cpp)

# Add source files to Semantics library
add_library(Semantics SymbolTable.h SymbolTableBuilder.h SymbolTableBuilder.cpp)
# Make Parser library visible to Semantics
target_link_libraries (Semantics Parser)

# Making the interpeter
set (target interp)
# interp as default project
set_property (DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${DEP_VS_DEFAULT_PROJECT})

add_executable(${target} interp.cpp)

target_link_libraries (${target} Parser)
#target_link_libraries (${target} Semantics)

install (TARGETS ${target} DESTINATION bin)

#Finding LLVM
# http://llvm.org/docs/CMake.html#embedding-llvm-in-your-project

# Allow the user to enter their LLVM install path
set(LLVM_INSTALL_PREFIX CACHE PATH "If LLVM was built using CMake, this is the location where it was installed")

# Update LLVM_DIR for find_package. FORCE makes the GUI update
if(EXISTS "${LLVM_INSTALL_PREFIX}/lib/cmake/llvm")
set(LLVM_DIR "${LLVM_INSTALL_PREFIX}/lib/cmake/llvm" CACHE PATH "LLVM CMake files" FORCE)
endif()

if(EXISTS "${LLVM_INSTALL_PREFIX}/share/llvm/cmake")
set(LLVM_DIR "${LLVM_INSTALL_PREFIX}/share/llvm/cmake" CACHE PATH "LLVM CMake files" FORCE)
endif()

find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})