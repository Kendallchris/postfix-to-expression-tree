cmake_minimum_required(VERSION 3.10)
project(postfix_to_expression_tree_assignment)

set(CMAKE_CXX_STANDARD 14)

add_executable(expr-tree main.cpp tree.cpp tree.h tree_node.cpp tree_node.h)
