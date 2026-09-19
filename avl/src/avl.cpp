#include "avl.hpp"
#include "visualizer.hpp"
#include <algorithm>

AVL::AVL() : root(nullptr) {}

AVL::~AVL() {
    destroyTree(root);
}

void AVL::destroyTree(AVLNode* node) {
    if (node) {
        destroyTree(node->left);
        destroyTree(node->right);
        delete node;
    }
}

Metrics AVL::getMetrics() const {
    return metrics;
}

void AVL::resetMetrics() {
    metrics.reset();
}

int AVL::getHeight(AVLNode* node) const {
    return node ? node->height : 0;
}

int AVL::getBalance(AVLNode* node) const {
    return node ? getHeight(node->left) - getHeight(node->right) : 0;
}

void AVL::updateHeight(AVLNode* node) {
    if (node) {
        node->height = 1 + std::max(getHeight(node->left), getHeight(node->right));
    }
}

AVLNode* AVL::rotateRight(AVLNode* y) {
    metrics.rotations++;
    AVLNode* x = y->left;
    AVLNode* T2 = x->right;

    x->right = y;
    y->left = T2;

    updateHeight(y);
    updateHeight(x);

    return x;
}

AVLNode* AVL::rotateLeft(AVLNode* x) {
    metrics.rotations++;
    AVLNode* y = x->right;
    AVLNode* T2 = y->left;

    y->left = x;
    x->right = T2;

    updateHeight(x);
    updateHeight(y);

    return y;
}

void AVL::insert(int key) {
    root = insertNode(root, key);
}

AVLNode* AVL::insertNode(AVLNode* node, int key) {
    if (!node) {
        metrics.node_allocations++;
        return new AVLNode(key);
    }

    metrics.comparisons++;
    if (key < node->key) {
        node->left = insertNode(node->left, key);
    } else if (key > node->key) {
        metrics.comparisons++;
        node->right = insertNode(node->right, key);
    } else {
        metrics.comparisons++;
        return node; // duplicado
    }

    updateHeight(node);
    int balance = getBalance(node);

    // Left Left
    if (balance > 1 && key < node->left->key)
        return rotateRight(node);

    // Right Right
    if (balance < -1 && key > node->right->key)
        return rotateLeft(node);

    // Left Right
    if (balance > 1 && key > node->left->key) {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }

    // Right Left
    if (balance < -1 && key < node->right->key) {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }

    return node;
}

bool AVL::search(int key) {
    return searchNode(root, key);
}

bool AVL::searchNode(AVLNode* node, int key) {
    if (!node) return false;
    
    metrics.comparisons++;
    if (key == node->key) return true;
    
    metrics.comparisons++;
    if (key < node->key) {
        return searchNode(node->left, key);
    } else {
        return searchNode(node->right, key);
    }
}

void AVL::getNodesDot(AVLNode* node, std::string& out) const {
    if (!node) return;
    out += "    node" + std::to_string(node->key) + " [label=\"" + std::to_string(node->key) + "\\nh=" + std::to_string(node->height) + "\"];\n";
    getNodesDot(node->left, out);
    getNodesDot(node->right, out);
}

void AVL::getEdgesDot(AVLNode* node, std::string& out) const {
    if (!node) return;
    if (node->left) {
        out += "    node" + std::to_string(node->key) + " -> node" + std::to_string(node->left->key) + ";\n";
        getEdgesDot(node->left, out);
    }
    if (node->right) {
        out += "    node" + std::to_string(node->key) + " -> node" + std::to_string(node->right->key) + ";\n";
        getEdgesDot(node->right, out);
    }
}

void AVL::exportDot(const std::string& filename, const std::string& title) const {
    std::string dot = "digraph AVL {\n";
    dot += "    labelloc=\"t\";\n";
    dot += "    label=\"" + title + "\";\n";
    dot += "    node [shape=circle, style=filled, fillcolor=lightgreen, fontname=\"Arial\"];\n";
    
    if (!root) {
        dot += "    empty [label=\"Vazia\", shape=none, fillcolor=none];\n";
    } else {
        getNodesDot(root, dot);
        getEdgesDot(root, dot);
    }
    
    dot += "}\n";
    std::ofstream out(filename); out << dot; out.close();
}
