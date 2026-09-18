#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <fstream>
#include "metrics.hpp"

struct PatriciaNode {
    std::unordered_map<char, PatriciaNode*> children;
    std::string edge_label;
    bool is_end_of_word = false;
    std::string word_value = "";

    PatriciaNode(const std::string& label = "") : edge_label(label) {}
    ~PatriciaNode() = default;
};

class PatriciaTree {
private:
    PatriciaNode* root;
    size_t word_count;
    size_t total_nodes;
    Metrics metrics;

    void clearHelper(PatriciaNode* node);
    bool removeHelper(PatriciaNode* current, const std::string& word, size_t strIdx, bool& wordFound);
    void collectWordsHelper(PatriciaNode* node, std::vector<std::string>& results) const;
    size_t commonPrefixLength(const std::string& s1, const std::string& s2) const;

public:
    PatriciaTree();
    ~PatriciaTree();

    // Fundamental operations
    bool insert(const std::string& word);
    bool search(const std::string& word);
    bool remove(const std::string& word);

    // Prefix operations
    bool startsWith(const std::string& prefix);
    std::vector<std::string> wordsWithPrefix(const std::string& prefix);
    std::string longestPrefixOf(const std::string& query);

    // Utility & Metrics
    size_t size() const { return word_count; }
    size_t nodeCount() const { return total_nodes; }
    bool empty() const { return word_count == 0; }
    void clear();
    const Metrics& getMetrics() const { return metrics; }
    void resetMetrics() { metrics.reset(); }

    // Visualization
    bool exportDot(const std::string& filename, const std::string& graphTitle = "Patricia Tree") const;
};
