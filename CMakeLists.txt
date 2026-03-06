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

set(INCLUDE_DIR include)
set(SRC_DIR src)

set(HEADER_FILES
	${INCLUDE_DIR}/jit.hpp
	${INCLUDE_DIR}/node.hpp
	${INCLUDE_DIR}/repl.hpp
	${INCLUDE_DIR}/lexer.hpp
	${INCLUDE_DIR}/token.hpp
	${INCLUDE_DIR}/types.hpp
	${INCLUDE_DIR}/object.hpp
	${INCLUDE_DIR}/parser.hpp
	${INCLUDE_DIR}/context.hpp
	${INCLUDE_DIR}/helpers.hpp
	${INCLUDE_DIR}/visitor.hpp
	${INCLUDE_DIR}/ast_view.hpp
	${INCLUDE_DIR}/evaluator.hpp
	${INCLUDE_DIR}/exception.hpp
	${INCLUDE_DIR}/token_type.hpp
	${INCLUDE_DIR}/inst_visitor.hpp
	${INCLUDE_DIR}/operator_visitor.hpp

	${INCLUDE_DIR}/Node.hpp
	${INCLUDE_DIR}/Value.hpp
	${INCLUDE_DIR}/NodeDef.hpp
	${INCLUDE_DIR}/Operator.hpp
	${INCLUDE_DIR}/ValueDef.hpp
	${INCLUDE_DIR}/ASTContext.hpp
	${INCLUDE_DIR}/EvaluatorContext.hpp
)

set(SOURCE_FILES
	${SRC_DIR}/node.cpp
	${SRC_DIR}/repl.cpp
	${SRC_DIR}/lexer.cpp
	${SRC_DIR}/token.cpp
	${SRC_DIR}/types.cpp
	${SRC_DIR}/parser.cpp
	${SRC_DIR}/object.cpp
	${SRC_DIR}/context.cpp
	${SRC_DIR}/helpers.cpp
	${SRC_DIR}/visitor.cpp
	${SRC_DIR}/ast_view.cpp
	${SRC_DIR}/exception.cpp
	${SRC_DIR}/evaluator.cpp
	${SRC_DIR}/operator_visitor.cpp
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
