cmake_minimum_required(VERSION 3.6)
project(Project-CodeDemo)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -std=c++0x")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ./cppFiles/) # 可执行文件的输出目录

aux_source_directory(./cppFiles/ cpp_LIST)
aux_source_directory(./hFiles/ h_LIST)
add_executable(run ${h_LIST} ${cpp_LIST})
