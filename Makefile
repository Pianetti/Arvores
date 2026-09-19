CXX = g++
CXXFLAGS = -std=c++17 -O3 -Wall -Wextra -Icommon/include -Itrie/include -Ipatricia/include -Isplay/include -Itreap/include -Ikdtree/include -Ibst/include -Iavl/include

BIN_DIR = bin

COMMON_SRC = common/src/dataset_generator.cpp

all: trie patricia splay treap kdtree experimentos

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

trie: $(BIN_DIR)
	$(CXX) $(CXXFLAGS) trie/src/trie.cpp trie/src/main_trie.cpp -o $(BIN_DIR)/trie_demo

patricia: $(BIN_DIR)
	$(CXX) $(CXXFLAGS) patricia/src/patricia.cpp patricia/src/main_patricia.cpp -o $(BIN_DIR)/patricia_demo

splay: $(BIN_DIR)
	$(CXX) $(CXXFLAGS) splay/src/splay_tree.cpp splay/src/main_splay.cpp -o $(BIN_DIR)/splay_demo

treap: $(BIN_DIR)
	$(CXX) $(CXXFLAGS) treap/src/treap.cpp treap/src/main_treap.cpp -o $(BIN_DIR)/treap_demo

kdtree: $(BIN_DIR)
	$(CXX) $(CXXFLAGS) kdtree/src/kdtree.cpp bst/src/bst.cpp avl/src/avl.cpp kdtree/src/main_kdtree.cpp -o $(BIN_DIR)/kdtree_demo

experimentos: $(BIN_DIR)
	$(CXX) $(CXXFLAGS) experimentos/src/benchmark_runner.cpp trie/src/trie.cpp patricia/src/patricia.cpp splay/src/splay_tree.cpp treap/src/treap.cpp kdtree/src/kdtree.cpp bst/src/bst.cpp avl/src/avl.cpp $(COMMON_SRC) -o $(BIN_DIR)/benchmark

clean:
	rm -rf $(BIN_DIR) build *.dot *.svg

.PHONY: all clean trie patricia splay treap kdtree experimentos
