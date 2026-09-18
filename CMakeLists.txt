cmake_minimum_required(VERSION 3.16)
project(AEDS2_Arvores CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall -Wextra")

include_directories(
    common/include
    trie/include
    patricia/include
    splay/include
    treap/include
    kdtree/include
)

# Common utility library
add_library(common_lib common/src/dataset_generator.cpp)

# 1. Trie
add_library(trie_lib trie/src/trie.cpp)
add_executable(trie_demo trie/src/main_trie.cpp)
target_link_libraries(trie_demo trie_lib)

# 2. Patricia
add_library(patricia_lib patricia/src/patricia.cpp)
add_executable(patricia_demo patricia/src/main_patricia.cpp)
target_link_libraries(patricia_demo patricia_lib)

# 3. Splay
add_library(splay_lib splay/src/splay_tree.cpp)
add_executable(splay_demo splay/src/main_splay.cpp)
target_link_libraries(splay_demo splay_lib)

# 4. Treap
add_library(treap_lib treap/src/treap.cpp)
add_executable(treap_demo treap/src/main_treap.cpp)
target_link_libraries(treap_demo treap_lib)

# 5. KD-Tree
add_library(kdtree_lib kdtree/src/kdtree.cpp)
add_executable(kdtree_demo kdtree/src/main_kdtree.cpp)
target_link_libraries(kdtree_demo kdtree_lib)

# Experiments/Benchmark
add_executable(benchmark experimentos/src/benchmark_runner.cpp)
target_link_libraries(benchmark
    trie_lib
    patricia_lib
    splay_lib
    treap_lib
    kdtree_lib
    common_lib
)
