# Grammar and transformations

## Scope
This is a deliberately small C-like language for function definitions. It is a teaching front-end, not a full C compiler. Supported types are `int`, `float`, and `void`; parameters are typed; bodies contain return statements and simple typed declarations. Return expressions use identifiers/numbers with arithmetic operators.

## EBNF
```ebnf
Program      = { Function } ;
Function     = Type, Identifier, "(", [ Parameters ], ")", Block ;
Type         = "int" | "float" | "void" ;
Parameters   = Parameter, { ",", Parameter } ;
Parameter    = Type, Identifier ;
Block        = "{", { Statement }, "}" ;
Statement    = "return", [ Expression ], ";"
             | ("int" | "float"), Identifier, ";" ;
Expression   = Primary, { ("+" | "-" | "*" | "/"), Primary } ;
Primary      = Identifier | Number | "(", Expression, ")" ;
```

## CFG form
```text
Program       → Function Program | ε
Function      → Type ID ( Parameters ) Block
Type          → int | float | void
Parameters    → Parameter Tail | ε
Tail          → , Parameter Tail | ε
Parameter     → Type ID
Block         → { Statements }
Statements    → Statement Statements | ε
Statement     → return Expression ;
              | return ;
              | int ID ;
              | float ID ;
Expression    → Primary ExpressionTail
ExpressionTail→ + Primary ExpressionTail
              | - Primary ExpressionTail
              | * Primary ExpressionTail
              | / Primary ExpressionTail
              | ε
Primary       → ID | NUMBER | ( Expression )
```

## Grammar transformations
- The optional parameter list is expressed as `Parameters → Parameter Tail | ε`, removing the optional-list notation from the CFG.
- Repeated statements are expressed as `Statements → Statement Statements | ε`.
- Repeated arithmetic operator/operand pairs are expressed using `ExpressionTail`, avoiding left recursion in the expression rule.
- This grammar does not encode conventional operator precedence in the small demonstration parser. Parentheses are accepted by the browser demo. Treat this as a limitation rather than full C behaviour.
