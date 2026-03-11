CC = gcc
CFLAGS = -Iinclude -Wall -g
LDFLAGS = -lpthread

# Directories
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
DLL_DIR = C:/msys64/ucrt64/bin

# Target executable
TARGET = $(BIN_DIR)/sonu_vote.exe

# Source and Object Files
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

all: setup $(TARGET)

setup:
	@if not exist $(BIN_DIR) mkdir $(BIN_DIR)
	@if not exist $(OBJ_DIR) mkdir $(OBJ_DIR)
	@if not exist data mkdir data

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# New Release Command: Copies DLLs for sharing
release: all
	copy "$(DLL_DIR)\libwinpthread-1.dll" "$(BIN_DIR)\"
	copy "$(DLL_DIR)\libgcc_s_seh-1.dll" "$(BIN_DIR)\"
	@echo Release build complete in /bin folder.

clean:
	@if exist $(BIN_DIR) rd /s /q $(BIN_DIR)
	@if exist $(OBJ_DIR) rd /s /q $(OBJ_DIR)