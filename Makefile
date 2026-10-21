CC ?= gcc
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic -Iinclude
SRC = main.c src/lexer.c src/parser.c src/ast.c src/semantic.c
TARGET = mini-compiler

.PHONY: all clean test
all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

test: $(TARGET)
	./$(TARGET) tests/valid/sample.c
	-./$(TARGET) tests/invalid/undeclared_name.c

clean:
	rm -f $(TARGET)
