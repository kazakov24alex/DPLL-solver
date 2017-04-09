cmake_minimum_required(VERSION 3.7)
project(DPLL)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_EXE_LINKER_FLAGS " -static")

set(SOURCE_FILES dpll.cpp)
add_executable(DPLL ${SOURCE_FILES})
target_link_libraries(DPLL -static-libgcc -static-libstdc++)
