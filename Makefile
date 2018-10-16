CC := g++
FLAGS := -std=c++11 -w -g -pthread
INC_DIR := include
SRC_DIR := src
BUILD_DIR := build
BIN_DIR := bin
ASIO_DIR := /home/howard/asio-1.12.1/include
INCLUDE := -I./$(INC_DIR) -I$(ASIO_DIR)

$(BIN_DIR)/main: $(BUILD_DIR)/Date.o $(BUILD_DIR)/AgendaServer.o $(BUILD_DIR)/AgendaService.o $(BUILD_DIR)/AgendaUI.o $(BUILD_DIR)/User.o $(BUILD_DIR)/Meeting.o $(BUILD_DIR)/Storage.o $(BUILD_DIR)/main.o
	@mkdir -p $(BIN_DIR)
	$(CC) $(FLAGS) $(INCLUDE) $^ -o $@
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CC) $(FLAGS) $(INCLUDE) -c -o $@ $<

clean:
	@rm -rf $(BUILD_DIR)
	@rm -rf $(BIN_DIR)

