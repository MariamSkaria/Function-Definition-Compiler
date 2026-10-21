# Test cases

| Case | Input | Expected result |
|---|---|---|
| Valid: two parameters | `int add(int a, int b) { return a + b; }` | Lexer, parser, semantic checks pass |
| Valid: no parameters | `void greet() { return; }` | Pass |
| Valid: float return | `float half(float value) { return value / 2; }` | Pass |
| Invalid character | `int f() { return @; }` | Lexer reports invalid character |
| Missing parenthesis | `int f( { return 1; }` | Parser reports syntax error |
| Duplicate parameter | `int f(int x, int x) { return x; }` | Semantic error |
| Undeclared name | `int f() { return missing; }` | Semantic error |
| Void returning value | `void f() { return 1; }` | Semantic error |
