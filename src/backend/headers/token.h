#ifndef TOKEN
#define TOKEN

// standard library includes
#include <stdbool.h>

// definition of possible token types
typedef enum
{
    EMPTY = 0,
    NUMBER,
    OPERATOR,
    PARENTHESIS
} token_type;

// definitions for type implementations
// OPERATOR
typedef enum
{
    ADD = 1, SUB, MULT, DIV, MOD, POW, NEG,
    SIN, COS, TAN, ASIN, ACOS, ATAN,
    SIND, COSD, TAND, ASIND, ACOSD, ATAND,
    LN, LOG, ABS, FAC
} operator_type;

// operator properties
bool isunary(operator_type type);
unsigned int precedence(operator_type t);

// PARENTHESIS
typedef enum { LEFT = 1, RIGHT } parenthesis_type;

// Collect possible types in a union for storage in a Token
typedef union
{
    double number;
    operator_type operator;
    parenthesis_type parenthesis;
} TokenValue;

// token container
typedef struct
{
    token_type type;
    unsigned int column; // index of the first character in
                         // the input string used to build token
    TokenValue value;
} Token;

// Token creation functions
Token create_empty_token(void);
Token create_number_token(double value, unsigned int column);
Token create_operator_token(operator_type type, int column);
Token create_parenthesis_token(parenthesis_type type, int column);

// TOKEN LIST DATA STRUCTURE
typedef struct
{
    unsigned int count;
    unsigned int max;
    Token *list;
} TokenList;

// TOKEN LIST FUNCTION DECLARATIONS;
TokenList new_tokenlist();
void delete_tokenlist(TokenList tokenlist);
void clear_tokenlist(TokenList *self);
void tokenlist_add(TokenList *self, Token value);
Token tokenlist_pop(TokenList *self);

// memory state description functions
unsigned int tokenlist_allocation_count(void);

#endif // TOKEN
