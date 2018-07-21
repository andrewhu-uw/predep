cmake_minimum_required (VERSION 2.8)
set(INTERP_VERSION_MAJOR 0)
set(INTERP_VERSION_MINOR 1)

set (INTERP_VERSION "${INTERP_VERSION_MAJOR}.${INTERP_VERSION_MINOR}")

project (Predep)

# Pass CMake configurations into a real header
# (generates file in current binary directory)
configure_file (InterpConfig.h.in   InterpConfig.h)
# Install this file somewhere everybody can see
# (relative to CMAKE_INSTALL_PREFIX)
install ( FILES ${CMAKE_CURRENT_BINARY_DIR}/InterpConfig.h
          DESTINATION include/interp)

add_executable(interp interp.cpp)