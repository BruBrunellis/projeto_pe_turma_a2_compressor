# Compila somente src/*.c, com os headers de include/.
CC = gcc
CFLAGS = -std=c90 -pedantic-errors -Wall -Wextra -Iinclude

ifeq ($(OS),Windows_NT)
# Bibliotecas do Windows usadas pela tinyfiledialogs.
LDLIBS += -lcomdlg32 -lole32 -lgdi32
endif

BIN_DIR = bin
TARGET = $(BIN_DIR)/bitforest.exe

SOURCES = $(wildcard src/*.c)
HEADERS = $(wildcard include/*.h)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(SOURCES) $(HEADERS) Makefile | $(BIN_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $(SOURCES) -o $@ $(LDLIBS)

# Cria bin/ somente quando ela nao existe.
$(BIN_DIR):
	mkdir $(BIN_DIR)

ifeq ($(OS),Windows_NT)
clean:
	if exist $(BIN_DIR) rmdir /s /q $(BIN_DIR)
else
clean:
	rm -rf $(BIN_DIR)
endif
