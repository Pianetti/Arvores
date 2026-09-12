CXX = g++
CXXFLAGS = -std=c++17 -O3 -Wall -Wextra -Icommon/include -Itrie/include
BIN_DIR = bin

all: trie

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

trie: $(BIN_DIR)
	$(CXX) $(CXXFLAGS) trie/src/trie.cpp trie/src/main_trie.cpp -o $(BIN_DIR)/trie_demo

clean:
	rm -rf $(BIN_DIR) build

.PHONY: all clean trie
