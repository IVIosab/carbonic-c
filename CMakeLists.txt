cmake_minimum_required(VERSION 3.5)
project(Carbonic-C VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

find_package(FLEX)
find_package(BISON)
find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

include_directories(${LLVM_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_definitions(${LLVM_DEFINITIONS})

FLEX_TARGET(CarbonicLexer ${CMAKE_CURRENT_SOURCE_DIR}/lexical_analysis/lexer.l  ${CMAKE_BINARY_DIR}/lexer.cpp)
BISON_TARGET(CarbonicParser ${CMAKE_CURRENT_SOURCE_DIR}/syntax_analysis/parser.ypp ${CMAKE_BINARY_DIR}/parser.cpp)
ADD_FLEX_BISON_DEPENDENCY(CarbonicLexer CarbonicParser)

set(SOURCES
    lexer.cpp
    parser.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/driver/driver.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/printers/astPrinter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/printers/prettyPrinter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/semantic_analysis/semantic.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/code_generator/codeGenerator.cpp
    main.cpp
)

add_executable(carbonic_c ${SOURCES} ${FLEX_CarbonicLexer_OUTPUTS} ${BISON_CarbonicParser_OUTPUTS})

llvm_map_components_to_libnames(llvm_libs support core irreader)

target_compile_features(carbonic_c PUBLIC cxx_std_17)

target_link_libraries(carbonic_c ${llvm_libs})
