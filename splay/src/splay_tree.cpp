#include "splay_tree.hpp"
#include "visualizer.hpp"
#include <stdexcept>

SplayTree::SplayTree() : root(nullptr), node_count(0) {}

SplayTree::~SplayTree() {
    clear();
}

void SplayTree::clearHelper(SplayNode* node) {
    if (!node) return;
    clearHelper(node->left);
    clearHelper(node->right);
    delete node;
    metrics.node_deallocations++;
}

void SplayTree::clear() {
    clearHelper(root);
    root = nullptr;
    node_count = 0;
}

int SplayTree::getRootKey() const {
    if (!root) throw std::runtime_error("Árvore Splay vazia");
    return root->key;
}

void SplayTree::rotateLeft(SplayNode* x) {
    SplayNode* p = x->parent;
    if (!p) return;

    x->parent = p->parent;
    if (p->parent) {
        if (p == p->parent->left) {
            p->parent->left = x;
        } else {
            p->parent->right = x;
        }
    } else {
        root = x;
    }

    p->right = x->left;
    if (x->left) {
        x->left->parent = p;
    }

    x->left = p;
    p->parent = x;
    metrics.rotations++;
}

void SplayTree::rotateRight(SplayNode* x) {
    SplayNode* p = x->parent;
    if (!p) return;

    x->parent = p->parent;
    if (p->parent) {
        if (p == p->parent->left) {
            p->parent->left = x;
        } else {
            p->parent->right = x;
        }
    } else {
        root = x;
    }

    p->left = x->right;
    if (x->right) {
        x->right->parent = p;
    }

    x->right = p;
    p->parent = x;
    metrics.rotations++;
}

void SplayTree::splay(SplayNode* x) {
    if (!x) return;

    while (x->parent) {
        SplayNode* p = x->parent;
        SplayNode* g = p->parent;

        if (!g) {
            // Caso Zig simples
            if (x == p->left) {
                rotateRight(x);
            } else {
                rotateLeft(x);
            }
        } else if (p == g->left && x == p->left) {
            // Caso Zig-Zig (homólogo à esquerda: rotação em p, depois em x)
            rotateRight(p);
            rotateRight(x);
        } else if (p == g->right && x == p->right) {
            // Caso Zig-Zig (homólogo à direita: rotação em p, depois em x)
            rotateLeft(p);
            rotateLeft(x);
        } else if (p == g->left && x == p->right) {
            // Caso Zig-Zag (heterólogo: rotação em x, depois em x)
            rotateLeft(x);
            rotateRight(x);
        } else {
            // Caso Zig-Zag (heterólogo: rotação em x, depois em x)
            rotateRight(x);
            rotateLeft(x);
        }
    }
}

bool SplayTree::insert(int key) {
    if (!root) {
        root = new SplayNode(key);
        metrics.node_allocations++;
        node_count++;
        return true;
    }

    SplayNode* curr = root;
    SplayNode* p = nullptr;

    while (curr) {
        p = curr;
        metrics.comparisons++;
        if (key < curr->key) {
            curr = curr->left;
        } else if (key > curr->key) {
            curr = curr->right;
        } else {
            // Chave duplicada: splay do nó existente para a raiz
            splay(curr);
            return false;
        }
    }

    SplayNode* newNode = new SplayNode(key, p);
    metrics.node_allocations++;
    node_count++;

    if (key < p->key) {
        p->left = newNode;
    } else {
        p->right = newNode;
    }

    // Após inserção, splay do novo nó até a raiz
    splay(newNode);
    return true;
}

bool SplayTree::search(int key) {
    if (!root) return false;

    SplayNode* curr = root;
    SplayNode* lastVisited = root;

    while (curr) {
        lastVisited = curr;
        metrics.comparisons++;
        if (key < curr->key) {
            curr = curr->left;
        } else if (key > curr->key) {
            curr = curr->right;
        } else {
            // Elemento encontrado: splay até a raiz
            splay(curr);
            return true;
        }
    }

    // Elemento não encontrado: splay do último nó visitado
    splay(lastVisited);
    return false;
}

SplayNode* SplayTree::findMax(SplayNode* node) const {
    if (!node) return nullptr;
    while (node->right) {
        node = node->right;
    }
    return node;
}

bool SplayTree::remove(int key) {
    if (!search(key)) {
        return false; // Nó não existe (último visitado já sofreu splay)
    }

    // Agora o nó a ser removido é garantidamente a raiz (root->key == key)
    SplayNode* target = root;

    if (!root->left) {
        root = root->right;
        if (root) root->parent = nullptr;
    } else if (!root->right) {
        root = root->left;
        if (root) root->parent = nullptr;
    } else {
        SplayNode* leftTree = root->left;
        SplayNode* rightTree = root->right;
        leftTree->parent = nullptr;
        rightTree->parent = nullptr;

        // Encontra o maior elemento da subárvore esquerda e splay nele
        SplayNode* maxLeft = findMax(leftTree);
        
        // Temporariamente torna leftTree a raiz para aplicar splay
        root = leftTree;
        splay(maxLeft);

        // Agora root é maxLeft e root->right é obrigatoriamente nulo!
        root->right = rightTree;
        rightTree->parent = root;
    }

    delete target;
    metrics.node_deallocations++;
    node_count--;
    return true;
}

void SplayTree::exportDotHelper(std::ofstream& out, SplayNode* node) const {
    if (!node) return;

    // Estilo da raiz vs nós internos
    if (node == root) {
        out << "    n" << node->key << " [label=\"" << node->key 
            << " (root)\", shape=doublecircle, fillcolor=\"#FEF08A\", color=\"#CA8A04\"];\n";
    } else {
        out << "    n" << node->key << " [label=\"" << node->key 
            << "\", shape=circle, fillcolor=\"#EFF6FF\", color=\"#3B82F6\"];\n";
    }

    if (node->left) {
        out << "    n" << node->key << " -> n" << node->left->key << " [label=\" L \", color=\"#3B82F6\"];\n";
        exportDotHelper(out, node->left);
    }
    if (node->right) {
        out << "    n" << node->key << " -> n" << node->right->key << " [label=\" R \", color=\"#EF4444\"];\n";
        exportDotHelper(out, node->right);
    }
}

bool SplayTree::exportDot(const std::string& filename, const std::string& graphTitle) const {
    std::ofstream out(filename);
    if (!out.is_open()) return false;

    Visualizer::writeDotHeader(out, "SplayTree");
    out << "    labelloc=\"t\";\n";
    out << "    label=\"" << Visualizer::escapeLabel(graphTitle) << "\";\n";
    out << "    fontsize=14;\n\n";

    if (root) {
        exportDotHelper(out, root);
    }

    Visualizer::writeDotFooter(out);
    return true;
}
