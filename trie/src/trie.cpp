#include "trie.hpp"
#include "visualizer.hpp"
#include <queue>
#include <map>

Trie::Trie() : word_count(0), total_nodes(1) {
    root = new TrieNode();
    metrics.node_allocations++;
}

Trie::~Trie() {
    clear();
    delete root;
    metrics.node_deallocations++;
}

void Trie::clearHelper(TrieNode* node) {
    if (!node) return;
    for (auto& pair : node->children) {
        clearHelper(pair.second);
        delete pair.second;
        metrics.node_deallocations++;
    }
    node->children.clear();
}

void Trie::clear() {
    clearHelper(root);
    word_count = 0;
    total_nodes = 1;
}

bool Trie::insert(const std::string& word) {
    TrieNode* current = root;
    for (char c : word) {
        metrics.comparisons++;
        auto it = current->children.find(c);
        if (it == current->children.end()) {
            TrieNode* newNode = new TrieNode();
            metrics.node_allocations++;
            total_nodes++;
            current->children[c] = newNode;
            current = newNode;
        } else {
            current = it->second;
        }
    }
    if (!current->is_end_of_word) {
        current->is_end_of_word = true;
        current->word_value = word;
        word_count++;
        return true;
    }
    return false;
}

bool Trie::search(const std::string& word) {
    TrieNode* current = root;
    for (char c : word) {
        metrics.comparisons++;
        auto it = current->children.find(c);
        if (it == current->children.end()) {
            return false;
        }
        current = it->second;
    }
    return current != nullptr && current->is_end_of_word;
}

bool Trie::startsWith(const std::string& prefix) {
    TrieNode* current = root;
    for (char c : prefix) {
        metrics.comparisons++;
        auto it = current->children.find(c);
        if (it == current->children.end()) {
            return false;
        }
        current = it->second;
    }
    return current != nullptr;
}

void Trie::collectWordsHelper(TrieNode* node, std::vector<std::string>& results) const {
    if (!node) return;
    if (node->is_end_of_word) {
        results.push_back(node->word_value);
    }
    std::map<char, TrieNode*> sortedChildren(node->children.begin(), node->children.end());
    for (const auto& pair : sortedChildren) {
        collectWordsHelper(pair.second, results);
    }
}

std::vector<std::string> Trie::wordsWithPrefix(const std::string& prefix) {
    TrieNode* current = root;
    for (char c : prefix) {
        metrics.comparisons++;
        auto it = current->children.find(c);
        if (it == current->children.end()) {
            return {};
        }
        current = it->second;
    }
    std::vector<std::string> results;
    collectWordsHelper(current, results);
    return results;
}

size_t Trie::countWordsWithPrefix(const std::string& prefix) {
    return wordsWithPrefix(prefix).size();
}

bool Trie::removeHelper(TrieNode* current, const std::string& word, size_t depth, bool& wordFound) {
    if (!current) return false;

    if (depth == word.size()) {
        if (!current->is_end_of_word) {
            wordFound = false;
            return false;
        }
        current->is_end_of_word = false;
        current->word_value.clear();
        word_count--;
        wordFound = true;
        return current->children.empty();
    }

    char c = word[depth];
    metrics.comparisons++;
    auto it = current->children.find(c);
    if (it == current->children.end()) {
        wordFound = false;
        return false;
    }

    bool shouldDeleteChild = removeHelper(it->second, word, depth + 1, wordFound);

    if (shouldDeleteChild) {
        delete it->second;
        metrics.node_deallocations++;
        total_nodes--;
        current->children.erase(it);
        return !current->is_end_of_word && current->children.empty();
    }

    return false;
}

bool Trie::remove(const std::string& word) {
    bool wordFound = false;
    removeHelper(root, word, 0, wordFound);
    return wordFound;
}

bool Trie::exportDot(const std::string& filename, const std::string& graphTitle) const {
    std::ofstream out(filename);
    if (!out.is_open()) return false;

    Visualizer::writeDotHeader(out, "Trie");
    out << "    labelloc=\"t\";\n";
    out << "    label=\"" << Visualizer::escapeLabel(graphTitle) << "\";\n";
    out << "    fontsize=14;\n\n";

    std::unordered_map<TrieNode*, size_t> nodeIds;
    size_t nextId = 0;

    std::queue<TrieNode*> q;
    q.push(root);
    nodeIds[root] = nextId++;

    while (!q.empty()) {
        TrieNode* curr = q.front();
        q.pop();

        size_t currId = nodeIds[curr];
        if (curr == root) {
            out << "    node_" << currId << " [label=\"root\", shape=circle, fillcolor=\"#E2E8F0\"];\n";
        } else if (curr->is_end_of_word) {
            out << "    node_" << currId << " [label=\"" << Visualizer::escapeLabel(curr->word_value)
                << "\", shape=doublecircle, fillcolor=\"#DCFCE7\", color=\"#15803D\"];\n";
        } else {
            out << "    node_" << currId << " [label=\"\", shape=circle, width=0.25, height=0.25, fillcolor=\"#F1F5F9\"];\n";
        }

        std::map<char, TrieNode*> sortedChildren(curr->children.begin(), curr->children.end());
        for (const auto& pair : sortedChildren) {
            char edgeChar = pair.first;
            TrieNode* child = pair.second;

            if (nodeIds.find(child) == nodeIds.end()) {
                nodeIds[child] = nextId++;
                q.push(child);
            }
            size_t childId = nodeIds[child];
            out << "    node_" << currId << " -> node_" << childId 
                << " [label=\" " << edgeChar << " \"];\n";
        }
    }

    Visualizer::writeDotFooter(out);
    return true;
}
