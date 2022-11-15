#ifndef OPERATIONS
#define OPERATIONS

// project includes
#include "token.h"

// definition of possible error types
typedef enum
{
    SUCCESS = 0,
    MULTIPLE_DECIMAL_POINTS,
    NUM_ENDS_WITH_DOT,
    INVALID_INPUT_CHARACTER,
    INVALID_TOKEN,
    UNMATCHED_LEFT_PAR,
    UNMATCHED_RIGHT_PAR,
    ZERO_DIVISON,
    NEGATIVE_FRACTIONAL_EXPONENT,
    ZERO_NEGATIVE_EXPONENT,
    TANGENT_UNDEFINED,
    ARCUS_OUT_OF_RANGE,
    LOG_OUT_OF_RANGE,
    FAC_INPUT_NOT_INT
} error_type;

// operation return type
// error_index refers to characters in the input string
typedef struct
{
    error_type status;
    unsigned int error_index;
} ResultInfo;

// operations
ResultInfo lex (const char *input_string, TokenList *output);
ResultInfo syntax_check(const TokenList tokens);
ResultInfo convert(const char *input_string, TokenList *tokens);
ResultInfo parse(const char *input_string, Token *result);

#endif // OPERATIONS
