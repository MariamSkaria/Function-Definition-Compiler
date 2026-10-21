# Mini Compiler Project Report

## Title
Function Definition Compiler — Front-End Development

## Objective
To implement and demonstrate the main front-end phases of a compiler for a small C-like language: lexical analysis, syntax analysis, and basic semantic analysis.

## Problem statement
Function definitions contain structured information such as a return type, function name, parameters, and body. The project checks whether a limited set of function definitions follows a defined grammar and reports selected errors.

## Scope
The project accepts a deliberately small language with `int`, `float`, and `void` types; function definitions; typed parameters; simple variable declarations; return statements; numeric literals; identifiers; and arithmetic expressions. It is not a complete C compiler and does not generate machine code.

## Design
1. **Lexer:** converts source characters into tokens and records line/column locations.
2. **Parser:** checks function structure against the grammar and builds an in-memory function representation with expression nodes.
3. **Semantic analyzer:** checks duplicate function names, duplicate parameters, undeclared identifiers in return expressions, and void functions returning a value.
4. **Website demo:** lets a presenter edit sample source and view tokens, function summaries, and phase status.

## Grammar
See `grammar.md` for EBNF, CFG, and transformations.

## Testing
Use `tests/valid/sample.c` for valid input and files in `tests/invalid/` for invalid input. Record terminal and browser screenshots after running the tests.

## Limitations and future work
The language is intentionally restricted. Future improvements could add operator precedence, nested scopes, assignment expressions, complete return-type checking, a visual AST tree, and a browser interface connected to the C executable.

## Individual contribution details
Fill this table with the actual work completed by each group member. Do not claim contributions that were not made.

| Member | Actual contribution | Evidence |
|---|---|---|
| Member 1 | To be completed by group | Commits / files |
| Member 2 | To be completed by group | Commits / files |
| Member 3 | To be completed by group | Commits / files |
| Member 4 | To be completed by group | Commits / files |
| Member 5 | To be completed by group | Commits / files |
| Member 6 | To be completed by group | Commits / files |
