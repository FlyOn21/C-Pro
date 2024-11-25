cmake_minimum_required(VERSION 3.10)
project(CustomSharedPTR)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(SOURCE_FILES custom_shared_ptr.cpp, custom_shared_ptr.h)

add_executable(CustomSharedPTR custom_shared_ptr.cpp)