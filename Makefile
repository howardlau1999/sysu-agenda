CC := g++
FLAGS := -std=c++11 -w -g -pthread
INC_DIR := include
SRC_DIR := src
BUILD_DIR := build
BIN_DIR := bin
ASIO_DIR := /home/howard/asio-1.12.1/include
GTEST_DIR := ./gtest
INCLUDE := -I./$(INC_DIR) -I$(ASIO_DIR) -I$(GTEST_DIR)/include
TEST_LIBRARY := -L$(GTEST_DIR)/lib -lgtest -lgtest_main
$(BIN_DIR)/main: $(BUILD_DIR)/Date.o $(BUILD_DIR)/AgendaService.o $(BUILD_DIR)/AgendaUI.o $(BUILD_DIR)/User.o $(BUILD_DIR)/Meeting.o $(BUILD_DIR)/Storage.o $(BUILD_DIR)/main.o
	@mkdir -p $(BIN_DIR)
	$(CC) $(FLAGS) $(INCLUDE) $^ -o $@
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CC) $(FLAGS) $(INCLUDE) -c -o $@ $<

clean:
	@rm -rf $(BUILD_DIR)
	@rm -rf $(BIN_DIR)
	@rm -f  gtest-all.o gtest_main.o
gtest-all.o:
	g++ -isystem $(GTEST_DIR)/include -I$(GTEST_DIR) -pthread -c $(GTEST_DIR)/src/gtest-all.cc
gtest_main.o:
	g++ -isystem $(GTEST_DIR)/include -I$(GTEST_DIR) -pthread -c $(GTEST_DIR)/src/gtest_main.cc
$(GTEST_DIR)/lib/libgtest.a: gtest-all.o
	ar -rv $(GTEST_DIR)/lib/libgtest.a gtest-all.o
$(GTEST_DIR)/lib/libgtest_main.a:
	ar -rv $(GTEST_DIR)/lib/libgtest_main.a gtest_main.o
test: $(BUILD_DIR)/Date.o $(BUILD_DIR)/AgendaService.o $(BUILD_DIR)/AgendaUI.o $(BUILD_DIR)/User.o $(BUILD_DIR)/Meeting.o $(BUILD_DIR)/Storage.o $(GTEST_DIR)/lib/libgtest.a $(GTEST_DIR)/lib/libgtest_main.a
	@mkdir -p $(BIN_DIR)
	$(CC) $(FLAGS) $(INCLUDE) $^ test.cpp -o $@ $(TEST_LIBRARY)
