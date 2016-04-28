cmake_minimum_required(VERSION 2.8)

project(chat)

if(CMAKE_CXX_COMPILER_ID MATCHES GNU)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -fprofile-arcs -ftest-coverage")
    set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g3")
    set(CMAKE_CXX_FLAGS_RELEASE "-O2")
endif()

include_directories(
        ${PROJECT_SOURCE_DIR}/src
)

set(GOOGLETEST_ROOT lib/googletest/googletest CACHE STRING "Google Test source root")

include_directories(
        ${PROJECT_SOURCE_DIR}/${GOOGLETEST_ROOT}
        ${PROJECT_SOURCE_DIR}/${GOOGLETEST_ROOT}/include
)

set(GOOGLETEST_SOURCES
        ${PROJECT_SOURCE_DIR}/${GOOGLETEST_ROOT}/src/gtest-all.cc
        ${PROJECT_SOURCE_DIR}/${GOOGLETEST_ROOT}/src/gtest_main.cc)

foreach(_source ${GOOGLETEST_SOURCES})
    set_source_files_properties(${_source} PROPERTIES GENERATED 1)
endforeach()

add_library(googletest ${GOOGLETEST_SOURCES})

set(SOURCE_TEST_FILES test/main.cpp)
add_executable(unit_tests ${SOURCE_TEST_FILES})

add_dependencies(unit_tests googletest)

target_link_libraries(
        unit_tests
        googletest
        pthread
)

include(CTest)
enable_testing()

set(SOURCE_FILES src/server.cpp)
add_executable(server ${SOURCE_FILES})