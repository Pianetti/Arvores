with open("bst/src/bst.cpp", "r") as f:
    c = f.read()
c = c.replace("memoryAllocations", "node_allocations")
c = c.replace("Visualizer::saveDotFile(filename, dot);", "std::ofstream out(filename); out << dot; out.close();")
with open("bst/src/bst.cpp", "w") as f:
    f.write(c)

with open("avl/src/avl.cpp", "r") as f:
    c = f.read()
c = c.replace("memoryAllocations", "node_allocations")
c = c.replace("Visualizer::saveDotFile(filename, dot);", "std::ofstream out(filename); out << dot; out.close();")
with open("avl/src/avl.cpp", "w") as f:
    f.write(c)
