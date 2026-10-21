# Function-Definition-Compiler
A mini compiler front-end for function definitions implemented in C


to accept

int add(int a, int b) {
    return a + b;
}

Your compiler should be able to:
1. Read the function.
2. Convert it into tokens.
3. Check its syntax against the CFG.
4. Generate a parse tree or AST.
5. Check semantic rules, such as parameter names and return types.
6. Display whether the function is valid.
Use C for all compiler modules.

Member	Main responsibility	Files
1 Team Lead	CFG, shared interfaces and integration	compiler.h, grammar.md, main.c
2	Lexical analyzer	lexer.c, lexer.h
3	Syntax analyzer	parser.c, parser.h
4	Semantic analyzer	semantic.c, semantic.h
5	AST helpers and testing	ast.c, ast.h, tests/
6	Build system and documentation	Makefile, README.md, report and PPT

Ask everyone to create their own branch
Tell each member to clone the repository and create a separate branch.

For example, Member 2 should run:

git clone https://github.com/MariamSkaria/Function-Definition-Compiler.git
cd Function-Definition-Compiler
git checkout -b feature/lexer


Use the actual repository URL if yours differs.

Branch names for all members:
- Member 1: feature/grammar-integration
- Member 2: feature/lexer
- Member 3: feature/parser
- Member 4: feature/semantic
- Member 5: feature/ast-tests
- Member 6: feature/docs-build
Each member should commit and push their work on their own branch, then open a Pull Request for you to review.
