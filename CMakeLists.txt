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

# Testing
enable_testing()

file(GLOB_RECURSE invalid_tests "tests/inputs/invalid/*.crbc")
foreach(test ${invalid_tests})
    get_filename_component(testname ${test} NAME_WE)
    get_filename_component(testdir ${test} DIRECTORY)
    string(REPLACE "inputs" "outputs" output_dir ${testdir})
    set(output_file "${output_dir}/${testname}.out")
    add_test(NAME invalid/${testname} COMMAND sh -c "mkdir -p ${output_dir} && ./carbonic_c < ${test} > ${output_file}")
    set_tests_properties(invalid/${testname} PROPERTIES WILL_FAIL true)
endforeach()

file(GLOB_RECURSE component_tests "tests/inputs/components/*.crbc")
foreach(test ${component_tests})
    get_filename_component(testname ${test} NAME_WE)
    get_filename_component(testdir ${test} DIRECTORY)
    string(REPLACE "inputs" "outputs" output_dir ${testdir})
    set(output_file "${output_dir}/${testname}.out")
    add_test(NAME ${testdir}/${testname} COMMAND sh -c "mkdir -p ${output_dir} && ./carbonic_c < ${test} > ${output_file}")
endforeach()

file(GLOB_RECURSE component_tests "tests/inputs/code_generation/*.crbc")
foreach(test ${component_tests})
    get_filename_component(testname ${test} NAME_WE)
    get_filename_component(testdir ${test} DIRECTORY)
    string(REPLACE "inputs" "outputs" output_dir ${testdir})
    set(expected_output_init "${testdir}/${testname}.txt")
    string(REPLACE "programs" "expected_outputs" expected_output_file ${expected_output_init})
    set(output_file "${output_dir}/${testname}.out")
    set(generated_code_file "${output_dir}/${testname}.ll")
    add_test(NAME ${testdir}/${testname}
        COMMAND sh -c "mkdir -p ${output_dir} && ./carbonic_c < ${test} > ${output_file} 2> ${generated_code_file} && lli ${generated_code_file} | tee ${output_file}"
    )
    file(READ ${expected_output_file} expected_output)
    set_tests_properties(${testdir}/${testname}
        PROPERTIES PASS_REGULAR_EXPRESSION ${expected_output}
        OUTPUT_VARIABLE test_output
    )

endforeach()
