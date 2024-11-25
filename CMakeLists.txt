cmake_minimum_required(VERSION 3.24)
project(my-language VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(COMPILER_FLAGS "-Werror -Wextra -Wall")

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
)

set(SOURCE_FILES
    src/token.cpp
    src/lexer.cpp
)

add_executable(
lexer_example
    app/lexer_example.cpp
    ${SOURCE_FILES} ${HEADER_FILES}
)
target_link_libraries(lexer_example)

add_executable(
test_lexer
    tests/test_lexer.cpp
    ${SOURCE_FILES} ${HEADER_FILES}
)
target_link_libraries(test_lexer PRIVATE gtest gtest_main)

enable_testing()

add_test(NAME test_lexer COMMAND test_lexer)
