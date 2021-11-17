cmake_minimum_required(VERSION 3.20)
project(MySTL)

set(CMAKE_CXX_STANDARD 14)

add_executable(MySTL main.cpp construct.h alloc.h iterator.h type_traits.h vector.h list.h deque.h heap_algo.h priority_queue.h slist.h rb_tree.h stl_numeric.h stl_algobase.h stl_heap.h)
