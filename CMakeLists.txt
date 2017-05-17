cmake_minimum_required(VERSION 3.7)
project(c4_5)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

find_package (Threads)

set(SOURCE_FILES main.cpp prettyprint.hpp Haberman.cpp Haberman.h Node.cpp Node.h)
add_executable(c4_5 ${SOURCE_FILES})
target_link_libraries (c4_5 ${CMAKE_THREAD_LIBS_INIT})
