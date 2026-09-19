# Patch Makefile
with open('Makefile', 'r') as f:
    make = f.read()

if 'bst_lib' not in make and 'bst/include' not in make:
    make = make.replace('-Ikdtree/include', '-Ikdtree/include -Ibst/include -Iavl/include')
    make = make.replace('kdtree/src/kdtree.cpp', 'kdtree/src/kdtree.cpp bst/src/bst.cpp avl/src/avl.cpp')

with open('Makefile', 'w') as f:
    f.write(make)

# Patch CMakeLists.txt
with open('CMakeLists.txt', 'r') as f:
    cmake = f.read()

if 'bst/include' not in cmake:
    cmake = cmake.replace('kdtree/include', 'kdtree/include\\n    bst/include\\n    avl/include')

if 'bst_lib' not in cmake:
    bst_avl_cmake = """
# 6. BST
add_library(bst_lib bst/src/bst.cpp)

# 7. AVL
add_library(avl_lib avl/src/avl.cpp)
"""
    # insert before # Experiments/Benchmark
    cmake = cmake.replace('# Experiments/Benchmark', bst_avl_cmake + '\\n# Experiments/Benchmark')
    cmake = cmake.replace('kdtree_lib', 'kdtree_lib\\n    bst_lib\\n    avl_lib')

with open('CMakeLists.txt', 'w') as f:
    f.write(cmake)
