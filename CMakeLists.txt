cmake_minimum_required(VERSION 3.24)
project(pheonix VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(COMPILER_FLAGS "-Wextra -Wall -g -Wpedantic")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMPILER_FLAGS}")
elseif (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /WX")
endif()

find_package(LLVM REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")
include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})
llvm_map_components_to_libnames(llvm_libs
    core
    native
    orcjit
    support
    executionengine
)
add_definitions(${LLVM_DEFINITIONS})
include_directories(${LLVM_INCLUDE_DIRS})
link_directories(${LLVM_LIBRARY_DIRS})

set(COMPILER_FLAGS "-Werror -Wextra -Wall -g")

include(FetchContent)
FetchContent_Declare(
   googletest
   GIT_REPOSITORY      https://github.com/google/googletest.git
   GIT_TAG main
)
FetchContent_MakeAvailable(googletest)

include_directories(src)
include_directories(include)
include_directories(tests)
include_directories(app)

set(HEADER_FILES
    include/jit.hpp
    include/node.hpp
    include/repl.hpp
    include/lexer.hpp
    include/token.hpp
    include/types.hpp
    include/object.hpp
    include/parser.hpp
    include/context.hpp
    include/helpers.hpp
    include/visitor.hpp
    include/ast_view.hpp
    include/evaluator.hpp
    include/exception.hpp
    include/token_type.hpp
    include/inst_visitor.hpp
    include/operator_visitor.hpp

    include/Node.hpp
    include/Value.hpp
    include/NodeDef.hpp
    include/Operator.hpp
    include/ValueDef.hpp
    include/ASTContext.hpp
    include/EvaluatorContext.hpp

)

set(SOURCE_FILES
    src/node.cpp
    src/repl.cpp
    src/lexer.cpp
    src/token.cpp
    src/types.cpp
    src/parser.cpp
    src/object.cpp
    src/context.cpp
    src/helpers.cpp
    src/visitor.cpp
    src/ast_view.cpp
    src/exception.cpp
    src/evaluator.cpp
    src/operator_visitor.cpp
)

add_executable(
    example
    app/example.cpp
    ${SOURCE_FILES} ${HEADER_FILES}
)

add_executable(
    pheonix
    app/pheonix.cpp
    ${SOURCE_FILES} ${HEADER_FILES}
)

enable_testing()

add_executable(
    test_lexer
    tests/test_lexer.cpp
    ${SOURCE_FILES} ${HEADER_FILES}
)
target_link_libraries(test_lexer PRIVATE gtest gtest_main)

add_executable(
    test_parser
    tests/test_parser.cpp
    ${SOURCE_FILES} ${HEADER_FILES}
)
target_link_libraries(test_parser PRIVATE gtest gtest_main)

add_executable(
test_evaluator
    tests/test_evaluator.cpp
    ${SOURCE_FILES} ${HEADER_FILES}
)
target_link_libraries(test_evaluator PRIVATE gtest gtest_main)

add_executable(
TestEvaluator
	tests/TestEvaluator.cpp
    ${SOURCE_FILES} ${HEADER_FILES}
)
target_link_libraries(TestEvaluator PRIVATE gtest gtest_main)

add_test(NAME test_lexer COMMAND test_lexer)
add_test(NAME test_parser COMMAND test_parser)
add_test(NAME test_evaluator COMMAND test_evaluator)
add_test(NAME TestEvaluator COMMAND TestEvaluator)


add_executable(
test_jit
    tests/test_jit.cpp
    ${SOURCE_FILES} ${HEADER_FILES}
)
target_include_directories(test_jit PRIVATE ${LLVM_INCLUDE_DIRS})
target_compile_definitions(test_jit PRIVATE ${LLVM_DEFINITIONS})
target_link_libraries(test_jit ${llvm_libs})
