cmake_minimum_required(VERSION 3.28.1)
project("Calculator")

include_directories(${CMAKE_SOURCE_DIR}/inc)
link_directories(${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/out)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)

add_library(eval STATIC ${CMAKE_SOURCE_DIR}/src/eval.cpp)
target_link_libraries(eval math)
set_target_properties(eval PROPERTIES PREFIX "lib")
set_target_properties(eval PROPERTIES SUFFIX ".a")

add_executable(Calculator ${CMAKE_SOURCE_DIR}/src/mgsmbCalc.cpp)
target_link_libraries(Calculator eval)