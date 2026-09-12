cmake_minimum_required(VERSION 3.16)
project(AEDS2_Arvores CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall -Wextra")

include_directories(
    common/include
    trie/include
)

# 1. Trie
add_library(trie_lib trie/src/trie.cpp)
add_executable(trie_demo trie/src/main_trie.cpp)
target_link_libraries(trie_demo trie_lib)
