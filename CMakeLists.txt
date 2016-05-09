cmake_minimum_required(VERSION 2.8)

project(chat)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -fprofile-arcs -ftest-coverage")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g3")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")

include_directories(
        ${PROJECT_SOURCE_DIR}/src
)

add_library(
        server_options
        src/server_options.cpp
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

set(SOURCE_TEST_FILES test/main.cpp test/test_server_options.cpp)
add_executable(unit_tests ${SOURCE_TEST_FILES})

add_dependencies(unit_tests googletest)

target_link_libraries(
        unit_tests
        googletest
        server_options
        pthread
)

include(CTest)
enable_testing()

set(SERVER_SOURCE_FILES
        src/server.cpp
        src/server_options.cpp
        src/server_options.h
        src/options.h
        src/socket.cpp
        src/socket.h
        src/io_events.cpp
        src/io_events.h
        src/chat_server.cpp
        src/chat_server.h
        src/stream_reader.cpp
        src/stream_reader.h
        src/stream.h
        src/connection.cpp
        src/connection.h
        src/descriptor.h)

add_executable(server ${SERVER_SOURCE_FILES})

set(CLIENT_SOURCE_FILES
        src/client.cpp
        src/client_options.cpp
        src/client_options.h
        src/options.h)

add_executable(client ${CLIENT_SOURCE_FILES})