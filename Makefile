CXX = g++
CXXFLAGS = -std=c++17 -O3 -Wall -Wextra -Icommon/include
BIN_DIR = bin

all:
	@echo "Estrutura inicial e common/ configurados."

clean:
	rm -rf $(BIN_DIR) build

.PHONY: all clean
