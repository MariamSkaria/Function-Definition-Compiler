# Function Definition Compiler

A mini compiler front-end for a small C-like language. Includes a C command-line implementation (lexer, parser, AST structures, semantic checks) and a minimal responsive browser demo.

## Features
- Tokenizes keywords, identifiers, numbers, punctuation, arithmetic operators.
- Parses function definitions, typed parameters, function bodies, return statements and simple declarations.
- Performs basic semantic checks: duplicate function names, duplicate parameters, undeclared identifiers in return expressions, and void-return rules.
- Website playground shows token stream, function summaries, and stage results.

## Run the website
No dependencies or build tools are required.
1. Open `web/index.html` in a browser, or use VS Code Live Server.
2. Edit the example and click **Compile source**.

## Build the C command-line compiler
Requires GCC or Clang:
```bash
make
./mini-compiler tests/valid/sample.c
./mini-compiler tests/invalid/undeclared_name.c
```
The second command should return a non-zero exit status and report a semantic error.

## Supported language
See [`docs/grammar.md`](docs/grammar.md). This is intentionally a teaching subset, not a complete C compiler.

## Repository structure
- `include/`, `src/`: C compiler front-end.
- `web/`: user-friendly browser demo.
- `tests/`: sample valid and invalid programs.
- `docs/`: grammar and report support.

## Limitations
This mini-project does not generate machine code. It demonstrates front-end phases only. The C parser is intentionally limited and does not implement full C operator precedence, assignment statements, nested blocks, or complete type checking.
