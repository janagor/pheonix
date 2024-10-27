cmake_minimum_required(VERSION 3.10)
project(my-language VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(COMPILER_FLAGS "-Werror -Wextra -Wall")

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMPILER_FLAGS}")
elseif (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /WX")
endif()

find_package(Boost 1.86 CONFIG REQUIRED COMPONENTS unit_test_framework)
find_package(Boost 1.86 CONFIG REQUIRED COMPONENTS program_options)

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

include_directories(src)
include_directories(inc)
include_directories(tests)

set(SOURCE_FILES
    src/lexer.cpp
    src/repl.cpp
    src/parser.cpp
)

set(HEADER_FILES
    inc/repl.hpp
    inc/lexer.hpp
    inc/parser.hpp
)

enable_testing()
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    add_executable(
    my-language
        src/my-language.cpp
        ${SOURCE_FILES} ${HEADER_FILES}
    )
    target_link_libraries(my-language Boost::program_options)

    add_executable(
    test_lexer
        tests/test_lexer.cpp
        ${SOURCE_FILES} ${HEADER_FILES}
    )
    target_link_libraries(test_lexer Boost::unit_test_framework)

    add_executable(
    test_parser
        tests/test_parser.cpp
        ${SOURCE_FILES} ${HEADER_FILES}
    )
    target_link_libraries(test_parser Boost::unit_test_framework)

    add_test(NAME test_lexer COMMAND test_lexer)
    add_test(NAME test_parser COMMAND test_parser)
else()
    message(FATAL_ERROR "Boost not found!")
endif()
