#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>
#include <fstream>
#include "metrics.hpp"

struct TrieNode {
    std::unordered_map<char, TrieNode*> children;
    bool is_end_of_word = false;
    std::string word_value = "";

    TrieNode() = default;
    ~TrieNode() = default;
};

class Trie {
private:
    TrieNode* root;
    size_t word_count;
    size_t total_nodes;
    Metrics metrics;

    bool removeHelper(TrieNode* current, const std::string& word, size_t depth, bool& wordFound);
    void clearHelper(TrieNode* node);
    void collectWordsHelper(TrieNode* node, std::vector<std::string>& results) const;

public:
    Trie();
    ~Trie();

    // Fundamental operations
    bool insert(const std::string& word);
    bool search(const std::string& word);
    bool remove(const std::string& word);

    // Specific prefix queries
    bool startsWith(const std::string& prefix);
    std::vector<std::string> wordsWithPrefix(const std::string& prefix);
    size_t countWordsWithPrefix(const std::string& prefix);

    // Utility & Metrics
    size_t size() const { return word_count; }
    size_t nodeCount() const { return total_nodes; }
    bool empty() const { return word_count == 0; }
    void clear();
    const Metrics& getMetrics() const { return metrics; }
    void resetMetrics() { metrics.reset(); }

    // Visualisation
    bool exportDot(const std::string& filename, const std::string& graphTitle = "Trie") const;
};
