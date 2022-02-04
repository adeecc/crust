# Keywords

## Datatypes
KW_INT_32: i32
KW_INT_64: i64
KW_UINT_32: u32
KW_UINT_64: u64
KW_FLOAT_32: f32
KW_FLOAT_64: f64
KW_STRING: string
KW_BOOL: bool
KW_VOID: void
KW_TRUE: true
KW_FALSE: false

## Declarations
KW_LET: let
KW_CONST: const

# Conditions
KW_IF: if
KW_ELIF: elif
KW_ELSE: else

# Loops
KW_FOR: for
KW_WHILE: while
KW_BREAK: break
KW_CONTINUE: continue

# Functions
KW_FN: fn
KW_RETURN: return


# Modules
KW_IMPORT: import
KW_EXPORT: export


# Literals
INT_LITERAL: [0-9]+
FLOAT_LITERAL: [0-9]+.[0-9]*
STR_LITERAL: ".*"

# Identifier
IDENTIFIER: [a-zA-Z_][a-zA-Z0-9_]*

# Operators
OP_PLUS: +
OP_MINUS: -
OP_MULT: *
OP_DIV: /
OP_MOD: %
OP_AND: and
OP_OR: or
OP_NOT: not
OP_GT: >
OP_GE: >=
OP_EQ: ==
OP_NE: !=
OP_LE: <=
OP_LT: <

# Misc
SEMI_COLON: `;`
COLON: `:`
COMMA: `,`
RANGE: `..`
REFERNCE: `&`
NAMESPACE: `::`
LCURLYBRACE: '{'
RCURLYBRACE: '}'
LSQUAREBRACKET: '['
RSQUAREBRACKET: ']'
LPAREN: '('
RPAREN: ')'


# Comment
COMMENT: //.*