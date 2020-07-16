cmake_minimum_required(VERSION 2.6) 
project(Block5 C)
set(CMAKE_C_STANDARD 99)
add_executable(peer peer.c hashmap.c protokoll.c connection.c)