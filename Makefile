CC = gcc
CFLAGS = -Wall -O2
SRC_DIR = src
BUILD_DIR = build
BIN_DIR = bin

all: $(BIN_DIR)/fuzzy_search

$(BIN_DIR)/fuzzy_search: $(BUILD_DIR)/main.o $(BUILD_DIR)/bk_tree.o $(BUILD_DIR)/levenshtein.o
	mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $(BIN_DIR)/fuzzy_search $(BUILD_DIR)/main.o $(BUILD_DIR)/bk_tree.o $(BUILD_DIR)/levenshtein.o

$(BUILD_DIR)/main.o: $(SRC_DIR)/main.c $(SRC_DIR)/bk_tree.h $(SRC_DIR)/levenshtein.h
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $(SRC_DIR)/main.c -o $(BUILD_DIR)/main.o

$(BUILD_DIR)/bk_tree.o: $(SRC_DIR)/bk_tree.c $(SRC_DIR)/bk_tree.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/bk_tree.c -o $(BUILD_DIR)/bk_tree.o

$(BUILD_DIR)/levenshtein.o: $(SRC_DIR)/levenshtein.c $(SRC_DIR)/levenshtein.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/levenshtein.c -o $(BUILD_DIR)/levenshtein.o

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)
