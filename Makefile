CC = gcc
CFLAGS = -Wall -Wextra -Isrc/parser -Isrc/ast

SOURCES = src/parser/parser.tab.c \
          src/lexer/lex.yy.c \
          src/ast/ast.c \
          src/symbol_table/symbol_table.c \
          src/semantic/semantic.c \
          src/tac/tac.c

TARGET = ParseFlow.exe

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET)

clean:
	rm -f $(TARGET) *.o
