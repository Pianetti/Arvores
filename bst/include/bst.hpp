#ifndef BST_HPP
#define BST_HPP

#include "metrics.hpp"
#include <vector>
#include <string>

struct BSTNode {
    int key;
    BSTNode* left;
    BSTNode* right;
    
    BSTNode(int k) : key(k), left(nullptr), right(nullptr) {}
};

class BST {
private:
    BSTNode* root;
    Metrics metrics;

    void destroyTree(BSTNode* node);
    void getNodesDot(BSTNode* node, std::string& out) const;
    void getEdgesDot(BSTNode* node, std::string& out) const;

public:
    BST();
    ~BST();

    void insert(int key);
    bool search(int key);
    void remove(int key); // Opcional, usaremos principalmente ins/busca

    Metrics getMetrics() const;
    void resetMetrics();
    void exportDot(const std::string& filename, const std::string& title) const;
};

#endif
