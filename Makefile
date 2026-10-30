# Compilador
CC = gcc

# Opcoes de compilacao
CFLAGS = -Wall -Wextra -Wpedantic -std=c90 -Iinclude -g

# Diretorios
SRC_DIR = src
BUILD_DIR = build
BIN_DIR = bin

# Executavel final
TARGET = $(BIN_DIR)/compressor.exe

# Arquivos fonte
SOURCES = $(wildcard $(SRC_DIR)/*.c)

# Arquivos objeto
OBJECTS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SOURCES))

# Regra principal
.PHONY: all dirs clean

all: $(TARGET)

# Criacao dos diretorios
dirs:
	if not exist $(BUILD_DIR) mkdir $(BUILD_DIR)
	if not exist $(BIN_DIR) mkdir $(BIN_DIR)

# Geracao dos arquivos objeto
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | dirs
	$(CC) $(CFLAGS) -c $< -o $@

# Geracao do executavel
$(TARGET): $(OBJECTS) | dirs
	$(CC) $(OBJECTS) -o $(TARGET)

# Limpeza dos arquivos gerados
clean:
	if exist "$(BUILD_DIR)" rmdir /s /q "$(BUILD_DIR)"
	if exist "$(TARGET)" del /q "$(TARGET)"