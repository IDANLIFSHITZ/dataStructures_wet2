cmake_minimum_required(VERSION 3.24)
project(wet24_to_publish)

set(CMAKE_CXX_STANDARD 11)

add_executable(wet24_to_publish main.cpp hashTable.h hashTacble.cpp AVL.h Team.h Team.cpp Player.cpp LinkesList.h)
