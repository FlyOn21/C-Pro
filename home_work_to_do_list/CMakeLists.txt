cmake_minimum_required(VERSION 3.10)
project(TODOList)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(TODOList main.cpp task_controller.cpp task_manager.cpp task_viewer.cpp task.cpp)