CC = gcc
CFLAGS = -Iinclude -Wall -g
LDFLAGS = -lpthread

# Directories
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

# Target executable
TARGET = $(BIN_DIR)/sonu_vote.exe

# Correctly find all .c files and map them to .o files
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

all: setup $(TARGET)

setup:
	@if not exist $(BIN_DIR) mkdir $(BIN_DIR)
	@if not exist $(OBJ_DIR) mkdir $(OBJ_DIR)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@if exist $(BIN_DIR) rd /s /q $(BIN_DIR)
	@if exist $(OBJ_DIR) rd /s /q $(OBJ_DIR)