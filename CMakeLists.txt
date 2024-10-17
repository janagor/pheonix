cmake_minimum_required(VERSION 3.10)
project(my-language VERSION 1.0)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(Boost 1.86 COMPONENTS unit_test_framework)
find_package(Boost 1.86 REQUIRED COMPONENTS program_options)

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

include_directories(include)

set(SOURCE_FILES
    src/my-language.cpp
)

set(HEADER_FILES
    inc/lexer.hpp
)
# Tests
# enable_testing()
if(Boost_FOUND)
    # include_directories(${Boost_INCLUDE_DIRS})
    add_executable(test ${SOURCE_FILES} ${HEADER_FILES})

    target_link_libraries(test Boost::program_options)

else()
    message(FATAL_ERROR "Boost not found!")
endif()
