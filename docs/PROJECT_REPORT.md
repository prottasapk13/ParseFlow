# ParseFlow — Compiler Construction Lab Project Report

## Project Information

- **Project Name:** ParseFlow
- **Course:** Compiler Construction Lab
- **Implementation Language:** C
- **Lexer Generator:** Flex
- **Parser Generator:** GNU Bison
- **Development Environment:** Windows with MSYS2 UCRT64
- **Student Name:** Prottasa Chanda
- **Student ID:** 232-115-084
- **Institution:** Metropolitan University, Sylhet
- **Submission Date:** 29 November 2026

---

## 1. Introduction

ParseFlow is an educational compiler developed for the Compiler Construction Lab. It demonstrates the major phases of compiler construction: lexical analysis, syntax analysis, Abstract Syntax Tree construction, symbol table management, semantic analysis, and intermediate code generation.

The compiler accepts a small C-like programming language with variable declarations, assignments, expressions, conditional statements, loops, and print statements.

Flex is used to generate the lexical analyzer, GNU Bison is used to generate the parser, and the implementation is written in C.

## 2. Objectives

- Implement lexical analysis using Flex.
- Implement syntax analysis using GNU Bison.
- Construct and display an Abstract Syntax Tree.
- Maintain a symbol table with nested scope support.
- Detect semantic errors.
- Generate Three Address Code.
- Test valid and invalid programs.

## 3. Language Specification

### 3.1 Data Types

- `int` — integer values.
- `float` — decimal floating-point values.
- `bool` — Boolean values (`true` and `false`).

### 3.2 Supported Statements

- Variable declarations.
- Variable assignments.
- Print statements.
- If statements.
- If-else statements.
- While loops.
- Nested blocks.

### 3.3 Operators

| Category | Operators |
|---|---|
| Arithmetic | `+`, `-`, `*`, `/`, `%` |
| Relational | `<`, `>`, `<=`, `>=` |
| Equality | `==`, `!=` |
| Logical | `&&`, `||`, `!` |
| Assignment | `=` |

### 3.4 Comments

The lexer supports single-line comments beginning with `//` and block comments enclosed by `/*` and `*/`.

### 3.5 Identifiers and Literals

Identifiers begin with a letter or underscore. They may contain letters, digits, and underscores.

The lexer recognizes integer literals, decimal floating-point literals, and Boolean literals.

---

## 4. Context-Free Grammar

The parser supports the following statement grammar. Expression precedence and associativity are defined through Bison declarations.

```text

program -> optional_statements

optional_statements -> statements | epsilon

statements -> statement | statements statement

statement -> declaration ; | assignment ; | print_statement ; | if_statement | while_statement | block

declaration -> type IDENTIFIER | type IDENTIFIER = expression

type -> int | float | bool

assignment -> IDENTIFIER = expression

print_statement -> print ( expression ) | print IDENTIFIER

if_statement -> if ( expression ) statement | if ( expression ) statement else statement

while_statement -> while ( expression ) statement

block -> { optional_statements }

```

Expressions support literals, identifiers, parentheses, unary operators, arithmetic, relational, equality, and logical operators.

---

## 5. Compiler Architecture

ParseFlow consists of six major modules:

1. Lexical Analyzer (Flex)

2. Syntax Analyzer (Bison)

3. Abstract Syntax Tree (AST)

4. Symbol Table

5. Semantic Analyzer

6. Three Address Code Generator

### 5.1 Processing Flow

```text

Source Code

    |

    v

Lexical Analyzer (Flex)

    |

    v

Syntax Analyzer (Bison)

    |

    v

Abstract Syntax Tree

    |

    v

Symbol Table and Semantic Analysis

    |

    v

Three Address Code Generator

    |

    v

Generated TAC

```

Lexical and syntax errors prevent semantic analysis and TAC generation. Semantic errors also prevent TAC generation.

---

## 6. Lexical Analyzer

The lexical analyzer is implemented using Flex. It recognizes keywords, identifiers, integer and floating-point literals, Boolean literals, operators, delimiters, and comments.

It tracks source line numbers and reports invalid characters and unterminated block comments as lexical errors.

## 7. Syntax Analyzer

The syntax analyzer is implemented using GNU Bison. It checks source programs against the defined grammar and supports declarations, assignments, print statements, if-else statements, while loops, nested blocks, and expressions.

Operator precedence and associativity are defined using Bison declarations. Basic syntax error recovery is implemented at statement boundaries.

## 8. Abstract Syntax Tree

The AST represents the meaningful structure of the source program. ParseFlow uses nodes for programs, declarations, assignments, print statements, if statements, while loops, blocks, binary expressions, unary expressions, identifiers, and literals.

The AST is displayed after parsing and is used by semantic analysis and TAC generation.

---

## 9. Symbol Table

The symbol table stores information about declared variables.

| Field | Description |

|---|---|

| Name | Variable identifier |

| Type | Variable data type |

| Scope | Scope in which the variable is declared |

| Line Declared | Source line of declaration |

Nested scopes are supported. Variables declared in inner blocks have their own scope, and variable shadowing is handled.

## 10. Semantic Analysis

The semantic analyzer checks whether the program follows the language semantic rules.

Implemented checks include:

- Use of undeclared variables.

- Redeclaration in the same scope.

- Scope violations.

- Type mismatches and invalid assignments.

- Invalid arithmetic expressions.

- Invalid logical expressions.

- Invalid modulo operations.

- Invalid relational operations.

- Invalid unary operations.

Semantic errors are reported with source line numbers. TAC generation is skipped when semantic errors are detected.

---

## 11. Three Address Code Generation

The TAC generator produces an intermediate representation of the source program.

It supports arithmetic, relational and logical expressions, temporary variables, print statements, conditional statements, if-else statements, while loops, labels, and jumps.

Example source program:

```c

int a = 10;

int b = 20;

int c = a + b;

print c;

```

Example TAC:

```text

a = 10

b = 20

t1 = a + b

c = t1

print c

```

TAC is generated only when lexical, syntax, and semantic analysis complete without errors.

## 12. Challenges and Solutions

### Lexer and Parser Integration

Flex-generated tokens are connected to the Bison parser through the generated parser header.

### Operator Precedence

Bison precedence and associativity declarations ensure expressions are parsed in the intended order.

### Nested Scopes

The symbol table supports nested scopes and variable shadowing.

### Semantic Validation

AST traversal and symbol table information are used to detect invalid declarations, assignments, and expressions.

### Error Handling

Line tracking and separate error handling prevent later compiler phases from running when earlier phases fail.

---

## 13. Testing and Results

ParseFlow was tested using valid and invalid source programs.

The test categories include:

- Valid program execution.

- Invalid character detection.

- Unterminated block comment detection.

- Syntax error detection and recovery.

- Undeclared variable detection.

- Redeclaration detection.

- Scope violation detection.

- Type mismatch detection.

- Invalid arithmetic expressions.

- Invalid logical expressions.

- Nested scope and variable shadowing.

- TAC generation for arithmetic and control-flow statements.

Test input files and captured outputs are stored in the tests/ and tests/outputs/ directories.

Detailed test information is available in docs/TEST_RESULTS.md.

## 14. Conclusion

ParseFlow demonstrates the essential phases of compiler construction through a small C-like programming language.

The project integrates Flex and GNU Bison with AST construction, symbol table management, semantic analysis, and Three Address Code generation.

It provides practical experience with compiler design, grammar specification, error handling, scope management, and intermediate code generation.

## 15. References

1. Flex Manual: https://westes.github.io/flex/manual/

2. GNU Bison Manual: https://www.gnu.org/software/bison/manual/

3. Compiler Construction Lab Project Manual.

4. GCC Documentation: https://gcc.gnu.org/onlinedocs/
