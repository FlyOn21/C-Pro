cmake_minimum_required(VERSION 3.10)
project(VisitorPattern)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(VisitorPattern main.cpp Transaction.cpp TransactionVisitor.cpp)