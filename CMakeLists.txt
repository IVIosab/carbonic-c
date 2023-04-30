cmake_minimum_required(VERSION 3.5)
project(Carbonic-C VERSION 1.0.0)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

set(SOURCES
    lexer.cpp
    parser.cpp
    ./driver/driver.cpp
    ./printers/astPrinter.cpp
    ./printers/prettyPrinter.cpp
    ./semantic_analysis/semantic.cpp
    main.cpp
)

add_custom_command(
    OUTPUT lexer.cpp
    COMMAND flex -o lexer.cpp -i ${CMAKE_CURRENT_SOURCE_DIR}/lexical_analysis/lexer.l
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/lexical_analysis/lexer.l
    COMMENT "Generating lexer.cpp"
)

# add bison command
add_custom_command(
    OUTPUT parser.cpp parser.hpp
    COMMAND bison -d ${CMAKE_CURRENT_SOURCE_DIR}/syntax_analysis/parser.ypp -o parser.cpp
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/syntax_analysis/parser.ypp
    COMMENT "Generating parser.cpp and parser.hpp"
)

# add custom target
add_custom_target(
    my_custom_build
    DEPENDS lexer.cpp parser.cpp parser.hpp
)

add_executable(compile_carbonic ${SOURCES})

add_dependencies(compile_carbonic my_custom_build)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

