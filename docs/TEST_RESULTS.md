# ParseFlow Test Results

## Testing Environment
- Compiler: GCC
- Lexer: Flex
- Parser: Bison
- Platform: Windows with MSYS2 UCRT64

## Test Cases

| Test File | Purpose | Observed Result |
|---|---|---|
| lexical_error_test.txt | Invalid token detection | Lexical error detected; later phases skipped |
| syntax_error_test.txt | Syntax error handling | Syntax error recovered; later phases skipped |
| semantic_test.txt | Multiple semantic errors | 5 semantic errors reported |
| arithmetic_error_test.txt | Invalid arithmetic operands | Semantic error reported |
| logical_error_test.txt | Invalid logical operands | Semantic error reported |
| scope_violation_test.txt | Access outside declared scope | Undeclared variable reported |
| scope_test.txt | Scope-related semantic checking | Semantic error reported |
| nested_scope_test.txt | Nested scope handling | No semantic errors |
| shadow_test.txt | Variable shadowing | No semantic errors |
| valid_sample_test.txt | Complete manual sample program | Passed; no semantic errors and TAC generated |

## Additional Validation
- Manual sample program: passed.
- Boolean equality: passed.
- While-loop TAC generation: passed.
- If-else TAC generation: passed.
- Print TAC generation: passed.
- Makefile rebuild: passed.

## Output Files
Actual test outputs are stored in `tests/outputs/`.
