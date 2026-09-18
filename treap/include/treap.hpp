#pragma once
#include <iostream>
#include <fstream>
#include <random>
#include <string>
#include "metrics.hpp"

struct TreapNode {
    int key;
    int priority;
    TreapNode* left;
    TreapNode* right;

    TreapNode(int k, int p) 
        : key(k), priority(p), left(nullptr), right(nullptr) {}
    ~TreapNode() = default;
};

class Treap {
private:
    TreapNode* root;
    size_t node_count;
    std::mt19937 rng;
    std::uniform_int_distribution<int> priorityDist;
    Metrics metrics;

    TreapNode* rotateRight(TreapNode* y);
    TreapNode* rotateLeft(TreapNode* x);

    TreapNode* insertHelper(TreapNode* node, int key, int priority, bool& inserted);
    TreapNode* removeHelper(TreapNode* node, int key, bool& removed);
    void clearHelper(TreapNode* node);
    bool verifyHelper(TreapNode* node, int minKey, int maxKey) const;
    void exportDotHelper(std::ofstream& out, TreapNode* node) const;

public:
    Treap(uint32_t seed = 42);
    ~Treap();

    // Fundamental operations
    bool insert(int key);
    bool insert(int key, int priority); // Deterministic overload for demonstrations/tracing
    bool search(int key);
    bool remove(int key);

    // Advanced operations (Split / Merge)
    void split(int key, Treap& leftTreap, Treap& rightTreap);
    void merge(Treap& leftTreap, Treap& rightTreap);

    // Invariant verification (BST + Max-Heap)
    bool verifyInvariants() const;

    // Utility & Metrics
    size_t size() const { return node_count; }
    bool empty() const { return root == nullptr; }
    void clear();
    int getRootKey() const;
    int getRootPriority() const;

    const Metrics& getMetrics() const { return metrics; }
    void resetMetrics() { metrics.reset(); }

    // Visualization
    bool exportDot(const std::string& filename, const std::string& graphTitle = "Treap") const;
};
