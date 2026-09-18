#include "patricia.hpp"
#include "visualizer.hpp"
#include <queue>
#include <algorithm>
#include <map>

PatriciaTree::PatriciaTree() : word_count(0), total_nodes(1) {
    root = new PatriciaNode("");
    metrics.node_allocations++;
}

PatriciaTree::~PatriciaTree() {
    clear();
    delete root;
    metrics.node_deallocations++;
}

void PatriciaTree::clearHelper(PatriciaNode* node) {
    if (!node) return;
    for (auto& pair : node->children) {
        clearHelper(pair.second);
        delete pair.second;
        metrics.node_deallocations++;
    }
    node->children.clear();
}

void PatriciaTree::clear() {
    clearHelper(root);
    word_count = 0;
    total_nodes = 1;
}

size_t PatriciaTree::commonPrefixLength(const std::string& s1, const std::string& s2) const {
    size_t len = 0;
    size_t minLen = std::min(s1.size(), s2.size());
    while (len < minLen && s1[len] == s2[len]) {
        len++;
    }
    return len;
}

bool PatriciaTree::insert(const std::string& word) {
    if (word.empty()) {
        if (!root->is_end_of_word) {
            root->is_end_of_word = true;
            root->word_value = "";
            word_count++;
            return true;
        }
        return false;
    }

    PatriciaNode* current = root;
    size_t strIdx = 0;

    while (strIdx < word.size()) {
        char firstChar = word[strIdx];
        metrics.comparisons++;

        auto it = current->children.find(firstChar);
        if (it == current->children.end()) {
            std::string remaining = word.substr(strIdx);
            PatriciaNode* newNode = new PatriciaNode(remaining);
            newNode->is_end_of_word = true;
            newNode->word_value = word;
            metrics.node_allocations++;
            total_nodes++;
            current->children[firstChar] = newNode;
            word_count++;
            return true;
        }

        PatriciaNode* child = it->second;
        std::string remainingWord = word.substr(strIdx);
        size_t commonLen = commonPrefixLength(child->edge_label, remainingWord);
        metrics.comparisons += commonLen;

        if (commonLen < child->edge_label.size()) {
            metrics.edge_splits++;
            std::string commonPrefix = child->edge_label.substr(0, commonLen);
            std::string oldChildSuffix = child->edge_label.substr(commonLen);

            PatriciaNode* splitNode = new PatriciaNode(commonPrefix);
            metrics.node_allocations++;
            total_nodes++;

            child->edge_label = oldChildSuffix;
            splitNode->children[oldChildSuffix[0]] = child;
            current->children[firstChar] = splitNode;

            if (commonLen == remainingWord.size()) {
                splitNode->is_end_of_word = true;
                splitNode->word_value = word;
                word_count++;
                return true;
            } else {
                std::string newLeafSuffix = remainingWord.substr(commonLen);
                PatriciaNode* leafNode = new PatriciaNode(newLeafSuffix);
                leafNode->is_end_of_word = true;
                leafNode->word_value = word;
                metrics.node_allocations++;
                total_nodes++;
                splitNode->children[newLeafSuffix[0]] = leafNode;
                word_count++;
                return true;
            }
        }

        strIdx += child->edge_label.size();
        if (strIdx == word.size()) {
            if (!child->is_end_of_word) {
                child->is_end_of_word = true;
                child->word_value = word;
                word_count++;
                return true;
            }
            return false;
        }
        current = child;
    }

    return false;
}

bool PatriciaTree::search(const std::string& word) {
    if (word.empty()) return root->is_end_of_word;

    PatriciaNode* current = root;
    size_t strIdx = 0;

    while (strIdx < word.size()) {
        char firstChar = word[strIdx];
        metrics.comparisons++;

        auto it = current->children.find(firstChar);
        if (it == current->children.end()) {
            return false;
        }

        PatriciaNode* child = it->second;
        std::string remainingWord = word.substr(strIdx);
        if (remainingWord.size() < child->edge_label.size()) {
            return false;
        }

        if (remainingWord.compare(0, child->edge_label.size(), child->edge_label) != 0) {
            return false;
        }

        strIdx += child->edge_label.size();
        if (strIdx == word.size()) {
            return child->is_end_of_word;
        }
        current = child;
    }

    return false;
}

bool PatriciaTree::startsWith(const std::string& prefix) {
    if (prefix.empty()) return true;

    PatriciaNode* current = root;
    size_t strIdx = 0;

    while (strIdx < prefix.size()) {
        char firstChar = prefix[strIdx];
        metrics.comparisons++;

        auto it = current->children.find(firstChar);
        if (it == current->children.end()) {
            return false;
        }

        PatriciaNode* child = it->second;
        std::string remainingPrefix = prefix.substr(strIdx);

        if (remainingPrefix.size() <= child->edge_label.size()) {
            return child->edge_label.compare(0, remainingPrefix.size(), remainingPrefix) == 0;
        }

        if (remainingPrefix.compare(0, child->edge_label.size(), child->edge_label) != 0) {
            return false;
        }

        strIdx += child->edge_label.size();
        current = child;
    }

    return true;
}

void PatriciaTree::collectWordsHelper(PatriciaNode* node, std::vector<std::string>& results) const {
    if (!node) return;
    if (node->is_end_of_word) {
        results.push_back(node->word_value);
    }
    std::map<char, PatriciaNode*> sortedChildren(node->children.begin(), node->children.end());
    for (const auto& pair : sortedChildren) {
        collectWordsHelper(pair.second, results);
    }
}

std::vector<std::string> PatriciaTree::wordsWithPrefix(const std::string& prefix) {
    if (prefix.empty()) {
        std::vector<std::string> results;
        collectWordsHelper(root, results);
        return results;
    }

    PatriciaNode* current = root;
    size_t strIdx = 0;

    while (strIdx < prefix.size()) {
        char firstChar = prefix[strIdx];
        metrics.comparisons++;

        auto it = current->children.find(firstChar);
        if (it == current->children.end()) {
            return {};
        }

        PatriciaNode* child = it->second;
        std::string remainingPrefix = prefix.substr(strIdx);

        if (remainingPrefix.size() <= child->edge_label.size()) {
            if (child->edge_label.compare(0, remainingPrefix.size(), remainingPrefix) == 0) {
                std::vector<std::string> results;
                collectWordsHelper(child, results);
                return results;
            }
            return {};
        }

        if (remainingPrefix.compare(0, child->edge_label.size(), child->edge_label) != 0) {
            return {};
        }

        strIdx += child->edge_label.size();
        current = child;
    }

    std::vector<std::string> results;
    collectWordsHelper(current, results);
    return results;
}

std::string PatriciaTree::longestPrefixOf(const std::string& query) {
    PatriciaNode* current = root;
    std::string longest = "";
    if (root->is_end_of_word) longest = root->word_value;

    size_t strIdx = 0;
    while (strIdx < query.size()) {
        char firstChar = query[strIdx];
        auto it = current->children.find(firstChar);
        if (it == current->children.end()) break;

        PatriciaNode* child = it->second;
        std::string remaining = query.substr(strIdx);
        if (remaining.size() < child->edge_label.size()) {
            break;
        }

        if (remaining.compare(0, child->edge_label.size(), child->edge_label) != 0) {
            break;
        }

        strIdx += child->edge_label.size();
        if (child->is_end_of_word) {
            longest = child->word_value;
        }
        current = child;
    }

    return longest;
}

bool PatriciaTree::removeHelper(PatriciaNode* current, const std::string& word, size_t strIdx, bool& wordFound) {
    if (!current) return false;

    if (strIdx == word.size()) {
        if (!current->is_end_of_word) {
            wordFound = false;
            return false;
        }
        current->is_end_of_word = false;
        current->word_value.clear();
        word_count--;
        wordFound = true;
    } else {
        char firstChar = word[strIdx];
        auto it = current->children.find(firstChar);
        if (it == current->children.end()) {
            wordFound = false;
            return false;
        }

        PatriciaNode* child = it->second;
        std::string remaining = word.substr(strIdx);
        if (remaining.compare(0, child->edge_label.size(), child->edge_label) != 0) {
            wordFound = false;
            return false;
        }

        bool childRemoved = removeHelper(child, word, strIdx + child->edge_label.size(), wordFound);
        if (childRemoved) {
            delete child;
            metrics.node_deallocations++;
            total_nodes--;
            current->children.erase(firstChar);
        }
    }

    if (!wordFound) return false;

    // Se este nó não for raiz e não for terminal:
    // 1. Se não tem filhos, pode ser excluído pelo nó ancestral
    if (current != root && !current->is_end_of_word && current->children.empty()) {
        return true;
    }

    // 2. Se tem exatamente 1 filho e não é terminal, funde com seu único filho (Edge Merging)
    if (current != root && !current->is_end_of_word && current->children.size() == 1) {
        auto onlyChildPair = *current->children.begin();
        PatriciaNode* onlyChild = onlyChildPair.second;

        current->edge_label += onlyChild->edge_label;
        current->is_end_of_word = onlyChild->is_end_of_word;
        current->word_value = onlyChild->word_value;
        current->children = std::move(onlyChild->children);

        delete onlyChild;
        metrics.node_deallocations++;
        metrics.edge_merges++;
        total_nodes--;
    }

    return false;
}

bool PatriciaTree::remove(const std::string& word) {
    bool wordFound = false;
    removeHelper(root, word, 0, wordFound);
    return wordFound;
}

bool PatriciaTree::exportDot(const std::string& filename, const std::string& graphTitle) const {
    std::ofstream out(filename);
    if (!out.is_open()) return false;

    Visualizer::writeDotHeader(out, "PatriciaTree");
    out << "    labelloc=\"t\";\n";
    out << "    label=\"" << Visualizer::escapeLabel(graphTitle) << "\";\n";
    out << "    fontsize=14;\n\n";

    std::unordered_map<PatriciaNode*, size_t> nodeIds;
    size_t nextId = 0;

    std::queue<PatriciaNode*> q;
    q.push(root);
    nodeIds[root] = nextId++;

    while (!q.empty()) {
        PatriciaNode* curr = q.front();
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

        std::map<char, PatriciaNode*> sortedChildren(curr->children.begin(), curr->children.end());
        for (const auto& pair : sortedChildren) {
            PatriciaNode* child = pair.second;
            if (nodeIds.find(child) == nodeIds.end()) {
                nodeIds[child] = nextId++;
                q.push(child);
            }
            size_t childId = nodeIds[child];
            out << "    node_" << currId << " -> node_" << childId 
                << " [label=\" \\\"" << Visualizer::escapeLabel(child->edge_label) << "\\\" \", color=\"#2563EB\", fontcolor=\"#1D4ED8\"];\n";
        }
    }

    Visualizer::writeDotFooter(out);
    return true;
}
