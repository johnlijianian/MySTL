cmake_minimum_required(VERSION 3.20)
project(MySTL)

set(CMAKE_CXX_STANDARD 14)

add_executable(MySTL main.cpp construct.h alloc.h iterator.h type_traits.h)
