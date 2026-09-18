#include "treap.hpp"
#include "visualizer.hpp"
#include <climits>
#include <stdexcept>

Treap::Treap(uint32_t seed) 
    : root(nullptr), node_count(0), rng(seed), priorityDist(1, 1000000000) {}

Treap::~Treap() {
    clear();
}

void Treap::clearHelper(TreapNode* node) {
    if (!node) return;
    clearHelper(node->left);
    clearHelper(node->right);
    delete node;
    metrics.node_deallocations++;
}

void Treap::clear() {
    clearHelper(root);
    root = nullptr;
    node_count = 0;
}

int Treap::getRootKey() const {
    if (!root) throw std::runtime_error("Treap vazia");
    return root->key;
}

int Treap::getRootPriority() const {
    if (!root) throw std::runtime_error("Treap vazia");
    return root->priority;
}

TreapNode* Treap::rotateRight(TreapNode* y) {
    TreapNode* x = y->left;
    y->left = x->right;
    x->right = y;
    metrics.rotations++;
    return x;
}

TreapNode* Treap::rotateLeft(TreapNode* x) {
    TreapNode* y = x->right;
    x->right = y->left;
    y->left = x;
    metrics.rotations++;
    return y;
}

TreapNode* Treap::insertHelper(TreapNode* node, int key, int priority, bool& inserted) {
    if (!node) {
        inserted = true;
        metrics.node_allocations++;
        return new TreapNode(key, priority);
    }

    metrics.comparisons++;
    if (key == node->key) {
        inserted = false;
        return node;
    }

    if (key < node->key) {
        node->left = insertHelper(node->left, key, priority, inserted);
        // Preserva Max-Heap
        if (node->left->priority > node->priority) {
            node = rotateRight(node);
        }
    } else {
        node->right = insertHelper(node->right, key, priority, inserted);
        // Preserva Max-Heap
        if (node->right->priority > node->priority) {
            node = rotateLeft(node);
        }
    }

    return node;
}

bool Treap::insert(int key) {
    int prio = priorityDist(rng);
    return insert(key, prio);
}

bool Treap::insert(int key, int priority) {
    bool inserted = false;
    root = insertHelper(root, key, priority, inserted);
    if (inserted) node_count++;
    return inserted;
}

bool Treap::search(int key) {
    TreapNode* curr = root;
    while (curr) {
        metrics.comparisons++;
        if (key == curr->key) {
            return true;
        }
        if (key < curr->key) {
            curr = curr->left;
        } else {
            curr = curr->right;
        }
    }
    return false;
}

TreapNode* Treap::removeHelper(TreapNode* node, int key, bool& removed) {
    if (!node) {
        removed = false;
        return nullptr;
    }

    metrics.comparisons++;
    if (key < node->key) {
        node->left = removeHelper(node->left, key, removed);
    } else if (key > node->key) {
        node->right = removeHelper(node->right, key, removed);
    } else {
        // Encontrou o nó a remover
        removed = true;

        if (!node->left && !node->right) {
            // Caso 1: Folha
            delete node;
            metrics.node_deallocations++;
            return nullptr;
        } else if (!node->left) {
            // Caso 2: Apenas filho direito
            node = rotateLeft(node);
            node->left = removeHelper(node->left, key, removed);
        } else if (!node->right) {
            // Caso 3: Apenas filho esquerdo
            node = rotateRight(node);
            node->right = removeHelper(node->right, key, removed);
        } else {
            // Caso 4: Dois filhos - rotaciona com o de maior prioridade para manter Max-Heap
            if (node->left->priority > node->right->priority) {
                node = rotateRight(node);
                node->right = removeHelper(node->right, key, removed);
            } else {
                node = rotateLeft(node);
                node->left = removeHelper(node->left, key, removed);
            }
        }
    }

    return node;
}

bool Treap::remove(int key) {
    bool removed = false;
    root = removeHelper(root, key, removed);
    if (removed) node_count--;
    return removed;
}

bool Treap::verifyHelper(TreapNode* node, int minKey, int maxKey) const {
    if (!node) return true;

    // 1. Invariante BST
    if (node->key <= minKey || node->key >= maxKey) return false;

    // 2. Invariante Max-Heap
    if (node->left && node->left->priority > node->priority) return false;
    if (node->right && node->right->priority > node->priority) return false;

    return verifyHelper(node->left, minKey, node->key) && 
           verifyHelper(node->right, node->key, maxKey);
}

bool Treap::verifyInvariants() const {
    return verifyHelper(root, INT_MIN, INT_MAX);
}

void Treap::exportDotHelper(std::ofstream& out, TreapNode* node) const {
    if (!node) return;

    out << "    n" << node->key << " [shape=record, style=filled, fillcolor=\"#F0FDF4\", color=\"#16A34A\", label=\"{ "
        << "chave: " << node->key << " | prioridade: " << node->priority << " }\"];\n";

    if (node->left) {
        out << "    n" << node->key << " -> n" << node->left->key << " [label=\" L \", color=\"#2563EB\"];\n";
        exportDotHelper(out, node->left);
    }
    if (node->right) {
        out << "    n" << node->key << " -> n" << node->right->key << " [label=\" R \", color=\"#DC2626\"];\n";
        exportDotHelper(out, node->right);
    }
}

bool Treap::exportDot(const std::string& filename, const std::string& graphTitle) const {
    std::ofstream out(filename);
    if (!out.is_open()) return false;

    Visualizer::writeDotHeader(out, "Treap");
    out << "    labelloc=\"t\";\n";
    out << "    label=\"" << Visualizer::escapeLabel(graphTitle) << "\";\n";
    out << "    fontsize=14;\n\n";

    if (root) {
        exportDotHelper(out, root);
    }

    Visualizer::writeDotFooter(out);
    return true;
}
