# ParseFlow – Mini Programming Language Compiler

## 1. Project Overview

ParseFlow is a mini programming language compiler developed using C, Flex, and Bison as a Compiler Construction Lab project.

It demonstrates the major phases of a compiler, including lexical analysis, syntax analysis, Abstract Syntax Tree (AST) construction, symbol table management, semantic analysis, and Three Address Code (TAC) generation.

The compiler detects lexical, syntax, and semantic errors and skips TAC generation when semantic errors are found.

## 2. Features

- Lexical Analysis using Flex
- Syntax Analysis using Bison
- Abstract Syntax Tree (AST) construction
- Symbol Table Management
- Nested Scope Management
- Variable Declaration and Assignment
- Semantic Error Detection
- Type Checking
- Three Address Code (TAC) Generation
- Conditional Statements (`if-else`)
- Loop Statements (`while`)
- Arithmetic and Logical Expressions
- Error Reporting with Line Numbers

## 3. Technologies Used

- C Programming Language
- Flex
- Bison
- GCC Compiler
- MSYS2 UCRT64 Terminal
- Git and GitHub

## 4. Project Structure

```text
ParseFlow/
│
├── src/
│   ├── ast/
│   ├── lexer/
│   ├── parser/
│   ├── semantic/
│   ├── symbol_table/
│   └── tac/
│
├── tests/
│   └── outputs/
├── docs/
│   ├── PROJECT_REPORT.md
│   └── TEST_RESULTS.md
├── Makefile
├── README.md
└── ParseFlow.exe
```

## 5. Supported Language Features

### Data Types
- int
- float
- bool

### Statements
- Variable declaration
- Variable assignment
- Print statement
- If-else statement
- While loop
- Block statements

### Operators
- Arithmetic: `+`, `-`, `*`, `/`, `%`
- Relational: `<`, `>`, `<=`, `>=`
- Equality: `==`, `!=`
- Logical: `&&`, `||`, `!`

## 6. Requirements

Install the following tools:

- GCC
- Flex
- Bison

## 7. Build Instructions

Run these commands from the project root directory.

### Build Using Makefile (MSYS2 UCRT64)

```bash
mingw32-make
```

To force a complete rebuild:

```bash
mingw32-make -B
```

To clean the build:

```bash
mingw32-make clean
```

### Generate Parser Files

```bash
bison -d src/parser/parser.y -o src/parser/parser.tab.c
```
```bash
flex -o src/lexer/lex.yy.c src/lexer/lexer.l
```

### Compile the Project

```bash
gcc -Wall -Wextra -Isrc/parser -Isrc/ast src/parser/parser.tab.c src/lexer/lex.yy.c src/ast/ast.c src/symbol_table/symbol_table.c src/semantic/semantic.c src/tac/tac.c -o ParseFlow.exe
```

## 8. Run the Compiler

```bash
./ParseFlow.exe
```

Enter a program through the terminal. Use `Ctrl + D` to finish input.

## 9. Example Program

```c
int a = 10;
int b = 20;
int c = a + b;

if (c > 20) {
    print(c);
} else {
    print(0);
}
```

## 10. Sample Three Address Code

```text
a = 10
b = 20
t1 = a + b
c = t1
t2 = c > 20
ifFalse t2 goto L1
print c
goto L2
L1:
print 0
L2:
```

## 11. Error Handling

ParseFlow detects different types of errors:

- Lexical errors
- Syntax errors
- Undeclared variables
- Variable redeclaration
- Type mismatches
- Invalid expressions
- Invalid conditions

When semantic errors are detected, TAC generation is skipped.

## 12. Project Objective

The objective of ParseFlow is to understand and implement the fundamental phases of compiler construction through a small programming language developed using C, Flex, and Bison.
