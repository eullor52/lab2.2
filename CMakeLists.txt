cmake_minimum_required(VERSION 3.16)
project(lab2)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()

add_library(project_core INTERFACE)
target_include_directories(project_core INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/headers)

add_subdirectory(ui)
add_subdirectory(tests)