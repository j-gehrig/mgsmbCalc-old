cmake_minimum_required(VERSION 3.28.1)
project("Calculator")

include_directories(${CMAKE_SOURCE_DIR}/inc)
link_directories(${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/out)

add_executable(Calculator ${CMAKE_SOURCE_DIR}/src/calculator.cpp)
target_link_libraries(Calculator math)