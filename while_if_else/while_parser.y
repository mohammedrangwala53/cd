
%{
#include <stdio.h>
#include <stdlib.h>

/* Function declarations */
void yyerror(const char* s);
int yylex();
extern int lineno;  /* Line number from lexical analyzer */
extern char* yytext; /* Current token text */
int errors = 0;     /* Error counter */
%}

/* Token definitions */
%token WHILE LPAREN RPAREN LBRACE RBRACE
%token SEMICOLON COMMA ID NUM
%token AND OR NOT EQ NE LT GT LE GE
%token PLUS MINUS MULT DIV MOD ASSIGN
%token IF ELSE FOR RETURN

/* Operator precedence and associativity */
%left OR
%left AND
%left EQ NE
%left LT GT LE GE
%left PLUS MINUS
%left MULT DIV MOD
%right NOT
%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE

/* Start symbol */
%start program

%%

program
    : statement
    | program statement
    ;

statement
    : while_statement
    | other_statement
    ;

/* While statement grammar */
while_statement
    : WHILE LPAREN expression RPAREN compound_statement
    | WHILE LPAREN expression RPAREN simple_statement
    | WHILE error RPAREN compound_statement      { yyerror("Missing or invalid condition parenthesis"); }
    | WHILE LPAREN error RPAREN compound_statement { yyerror("Invalid condition expression"); }
    | WHILE LPAREN expression error compound_statement { yyerror("Missing closing parenthesis in while condition"); }
    | WHILE LPAREN RPAREN compound_statement     { yyerror("Empty condition in while statement"); }
    | WHILE LPAREN expression RPAREN error       { yyerror("Invalid body in while statement"); }
    ;

/* Compound statement (block) */
compound_statement
    : LBRACE statement_list RBRACE
    | LBRACE RBRACE
    | LBRACE error RBRACE          { yyerror("Invalid statement in block"); }
    | LBRACE statement_list error   { yyerror("Missing closing brace"); }
    ;

statement_list
    : statement
    | statement_list statement
    ;

/* Simple statement (non-compound) */
simple_statement
    : expression_statement
    | if_statement
    | for_statement
    | return_statement
    ;

expression_statement
    : expression SEMICOLON
    | SEMICOLON
    | expression error              { yyerror("Missing semicolon"); }
    ;

/* Various expression types */
expression
    : assignment_expression
    | logical_expression
    | relational_expression
    | arithmetic_expression
    | primary_expression
    ;

assignment_expression
    : ID ASSIGN expression
    | ID ASSIGN error               { yyerror("Invalid right-hand side in assignment"); }
    ;

logical_expression
    : expression AND expression
    | expression OR expression
    | NOT expression
    | expression AND error          { yyerror("Missing right operand in logical AND"); }
    | expression OR error           { yyerror("Missing right operand in logical OR"); }
    | NOT error                     { yyerror("Missing expression after NOT"); }
    ;

relational_expression
    : expression EQ expression
    | expression NE expression
    | expression LT expression
    | expression GT expression
    | expression LE expression
    | expression GE expression
    | expression EQ error           { yyerror("Missing right operand in equality comparison"); }
    | expression NE error           { yyerror("Missing right operand in inequality comparison"); }
    | expression LT error           { yyerror("Missing right operand in less than comparison"); }
    | expression GT error           { yyerror("Missing right operand in greater than comparison"); }
    | expression LE error           { yyerror("Missing right operand in less or equal comparison"); }
    | expression GE error           { yyerror("Missing right operand in greater or equal comparison"); }
    ;

arithmetic_expression
    : expression PLUS expression
    | expression MINUS expression
    | expression MULT expression
    | expression DIV expression
    | expression MOD expression
    | LPAREN expression RPAREN
    | PLUS expression
    | MINUS expression
    | expression PLUS error         { yyerror("Missing right operand in addition"); }
    | expression MINUS error        { yyerror("Missing right operand in subtraction"); }
    | expression MULT error         { yyerror("Missing right operand in multiplication"); }
    | expression DIV error          { yyerror("Missing right operand in division"); }
    | expression MOD error          { yyerror("Missing right operand in modulo operation"); }
    | LPAREN error                  { yyerror("Invalid expression in parentheses"); }
    | LPAREN expression error       { yyerror("Missing closing parenthesis"); }
    ;

primary_expression
    : ID
    | NUM
    | ID LPAREN argument_list RPAREN
    | ID LPAREN RPAREN
    | ID LPAREN error RPAREN        { yyerror("Invalid function arguments"); }
    | ID LPAREN argument_list error  { yyerror("Missing closing parenthesis in function call"); }
    ;

argument_list
    : expression
    | argument_list COMMA expression
    | argument_list COMMA error      { yyerror("Invalid argument in function call"); }
    ;

/* Other statement types (for context) */
if_statement
    : IF LPAREN expression RPAREN statement %prec LOWER_THAN_ELSE
    | IF LPAREN expression RPAREN statement ELSE statement
    ;

for_statement
    : FOR LPAREN expression_statement expression_statement RPAREN statement
    | FOR LPAREN expression_statement expression_statement expression RPAREN statement
    ;

return_statement
    : RETURN expression SEMICOLON
    | RETURN SEMICOLON
    ;

/* Placeholder for other statements */
other_statement
    : simple_statement
    | compound_statement
    ;

%%

void yyerror(const char* s) {
    fprintf(stderr, "Error at line %d: %s near '%s'\n", lineno, s, yytext);
    errors++;
}

int main() {
    yyparse();
    if (errors == 0) {
        printf("Parsing completed successfully.\n");
    } else {
        printf("Parsing completed with %d errors.\n", errors);
    }
    return 0;
}
