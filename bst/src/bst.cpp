#include "bst.hpp"
#include "visualizer.hpp"
#include <iostream>

BST::BST() : root(nullptr) {}

BST::~BST() {
    destroyTree(root);
}

void BST::destroyTree(BSTNode* node) {
    if (node) {
        destroyTree(node->left);
        destroyTree(node->right);
        delete node;
    }
}

Metrics BST::getMetrics() const {
    return metrics;
}

void BST::resetMetrics() {
    metrics.reset();
}

void BST::insert(int key) {
    BSTNode* newNode = new BSTNode(key);
    metrics.node_allocations++;

    if (!root) {
        root = newNode;
        return;
    }

    BSTNode* current = root;
    while (true) {
        metrics.comparisons++;
        if (key < current->key) {
            if (!current->left) {
                current->left = newNode;
                break;
            }
            current = current->left;
        } else if (key > current->key) {
            metrics.comparisons++; // Uma a mais para o >
            if (!current->right) {
                current->right = newNode;
                break;
            }
            current = current->right;
        } else {
            // Duplicado, ignora
            metrics.comparisons++;
            delete newNode;
            metrics.node_allocations--;
            break;
        }
    }
}

bool BST::search(int key) {
    BSTNode* current = root;
    while (current) {
        metrics.comparisons++;
        if (key == current->key) return true;
        
        metrics.comparisons++;
        if (key < current->key) {
            current = current->left;
        } else {
            current = current->right;
        }
    }
    return false;
}

void BST::remove(int key) {
    // Para simplificar a implementação pro benchmark (que foca em insert/search), 
    // faremos uma remoção iterativa ou recursiva básica.
    // Omitido para não alongar muito, a menos que necessário.
}

void BST::getNodesDot(BSTNode* node, std::string& out) const {
    if (!node) return;
    out += "    node" + std::to_string(node->key) + " [label=\"" + std::to_string(node->key) + "\"];\n";
    getNodesDot(node->left, out);
    getNodesDot(node->right, out);
}

void BST::getEdgesDot(BSTNode* node, std::string& out) const {
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

void BST::exportDot(const std::string& filename, const std::string& title) const {
    std::string dot = "digraph BST {\n";
    dot += "    labelloc=\"t\";\n";
    dot += "    label=\"" + title + "\";\n";
    dot += "    node [shape=circle, style=filled, fillcolor=lightblue, fontname=\"Arial\"];\n";
    
    if (!root) {
        dot += "    empty [label=\"Vazia\", shape=none, fillcolor=none];\n";
    } else {
        getNodesDot(root, dot);
        getEdgesDot(root, dot);
    }
    
    dot += "}\n";
    std::ofstream out(filename); out << dot; out.close();
}
