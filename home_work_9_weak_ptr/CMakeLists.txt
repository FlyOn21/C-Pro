cmake_minimum_required(VERSION 3.10)
project(UseWeakPtr)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(UseWeakPtr  use_weak_ptr.cpp course.cpp student.cpp)
