cmake_minimum_required(VERSION 3.24)
project(pheonix VERSION 1.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(COMPILER_FLAGS "-Werror -Wextra -Wall -g")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMPILER_FLAGS}")
elseif (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /WX")
endif()

include(FetchContent)
FetchContent_Declare( 
   googletest
   GIT_REPOSITORY      https://github.com/google/googletest.git
   GIT_TAG main
)
FetchContent_MakeAvailable(googletest)

include_directories(src)
include_directories(inc)
include_directories(tests)
include_directories(app)

set(HEADER_FILES
    inc/token.hpp
    inc/lexer.hpp
    # inc/repl.hpp
    inc/node.hpp
    inc/parser.hpp
    inc/visitor.hpp
    inc/operator_visitor.hpp
    inc/ast_view.hpp
    inc/evaluator.hpp
    inc/context.hpp
    inc/object.hpp
    inc/types.hpp
    inc/exception.hpp
    inc/helpers.hpp
    inc/token_type.hpp
)

set(SOURCE_FILES
    src/token.cpp
    src/lexer.cpp
    # src/repl.cpp
    src/node.cpp
    src/parser.cpp
    src/visitor.cpp
    src/operator_visitor.cpp
    src/ast_view.cpp
    src/evaluator.cpp
    src/context.cpp
    src/object.cpp
    src/types.cpp
    src/exception.cpp
    src/helpers.cpp
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

add_test(NAME test_lexer COMMAND test_lexer)
add_test(NAME test_parser COMMAND test_parser)
add_test(NAME test_evaluator COMMAND test_evaluator)
