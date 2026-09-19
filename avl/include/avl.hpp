#ifndef AVL_HPP
#define AVL_HPP

#include "metrics.hpp"
#include <vector>
#include <string>

struct AVLNode {
    int key;
    int height;
    AVLNode* left;
    AVLNode* right;
    
    AVLNode(int k) : key(k), height(1), left(nullptr), right(nullptr) {}
};

class AVL {
private:
    AVLNode* root;
    Metrics metrics;

    int getHeight(AVLNode* node) const;
    int getBalance(AVLNode* node) const;
    void updateHeight(AVLNode* node);

    AVLNode* rotateRight(AVLNode* y);
    AVLNode* rotateLeft(AVLNode* x);

    AVLNode* insertNode(AVLNode* node, int key);
    bool searchNode(AVLNode* node, int key);
    
    void destroyTree(AVLNode* node);
    void getNodesDot(AVLNode* node, std::string& out) const;
    void getEdgesDot(AVLNode* node, std::string& out) const;

public:
    AVL();
    ~AVL();

    void insert(int key);
    bool search(int key);

    Metrics getMetrics() const;
    void resetMetrics();
    void exportDot(const std::string& filename, const std::string& title) const;
};

#endif
