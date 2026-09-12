#pragma once
#include <cstdint>
#include <iostream>

struct Metrics {
    uint64_t comparisons = 0;
    uint64_t rotations = 0;
    uint64_t node_allocations = 0;
    uint64_t node_deallocations = 0;
    uint64_t edge_splits = 0;
    uint64_t edge_merges = 0;
    uint64_t nodes_visited = 0;

    void reset() {
        comparisons = 0;
        rotations = 0;
        node_allocations = 0;
        node_deallocations = 0;
        edge_splits = 0;
        edge_merges = 0;
        nodes_visited = 0;
    }

    void print(std::ostream& os = std::cout) const {
        os << "Comparações: " << comparisons << "\n"
           << "Rotações: " << rotations << "\n"
           << "Nós alocados: " << node_allocations << "\n"
           << "Nós desalocados: " << node_deallocations << "\n"
           << "Divisões de aresta: " << edge_splits << "\n"
           << "Fusões de aresta: " << edge_merges << "\n"
           << "Nós visitados: " << nodes_visited << "\n";
    }
};
