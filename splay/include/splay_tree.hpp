#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "metrics.hpp"

struct SplayNode {
    int key;
    SplayNode* left;
    SplayNode* right;
    SplayNode* parent;

    SplayNode(int k, SplayNode* p = nullptr) 
        : key(k), left(nullptr), right(nullptr), parent(p) {}
    ~SplayNode() = default;
};

class SplayTree {
private:
    SplayNode* root;
    size_t node_count;
    Metrics metrics;

    void rotateLeft(SplayNode* x);
    void rotateRight(SplayNode* x);
    void splay(SplayNode* x);

    void clearHelper(SplayNode* node);
    SplayNode* findMax(SplayNode* node) const;
    void exportDotHelper(std::ofstream& out, SplayNode* node) const;

public:
    SplayTree();
    ~SplayTree();

    // Fundamental operations
    bool insert(int key);
    bool search(int key);
    bool remove(int key);

    // Specific access and introspection
    int getRootKey() const;
    bool empty() const { return root == nullptr; }
    size_t size() const { return node_count; }
    void clear();

    const Metrics& getMetrics() const { return metrics; }
    void resetMetrics() { metrics.reset(); }

    // Visualization
    bool exportDot(const std::string& filename, const std::string& graphTitle = "Splay Tree") const;
};
