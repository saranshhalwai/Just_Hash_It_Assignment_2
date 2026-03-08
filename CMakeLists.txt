cmake_minimum_required(VERSION 4.2)
project(Just_Hash_It_Assignment_2 C)

set(CMAKE_C_STANDARD 11)

add_executable(Just_Hash_It_Assignment_2
        src/main.c
        src/rpc.c
        src/rpc.h
        src/bitcoin.c
        src/bitcoin.h)

target_link_libraries(Just_Hash_It_Assignment_2
        curl
        cjson)