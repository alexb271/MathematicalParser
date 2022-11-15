// standard library includes
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

// project includes
#include "token.h"
#include "parser.h"

#define BUFSIZE 256
#define PI 3.14159265358979323846264338327950288

// lex process variables
typedef struct
{
    error_type status;
} LexData;

static LexData init()
{
    LexData data;
    data.status = SUCCESS;
    return data;
}

static Token process_text(const char *input, unsigned int *input_idx, LexData *data)
{
    // record first input character as token starting column
    unsigned int column = *input_idx;

    // collect characters into a buffer
    // as long as they are lowercase letters
    char build_buffer[BUFSIZE];
    unsigned int build_buf_idx = 0;

    while (islower(input[*input_idx]))
    {
        build_buffer[build_buf_idx] = input[*input_idx];
        *input_idx += 1;

        // final spot in the array is reserved for \0
        if (build_buf_idx < BUFSIZE - 2)
            build_buf_idx += 1;
    }

    // set index to last processed character
    *input_idx -= 1;

    // add closing \0 to buffer
    build_buffer[build_buf_idx] = '\0';

    // build token based on string matching
    if (!strcmp(build_buffer, "sin"))
    {
        return create_operator_token(SIN, column);
    }
    else if (!strcmp(build_buffer, "cos"))
    {
        return create_operator_token(COS, column);
    }
    else if (!strcmp(build_buffer, "tan"))
    {
        return create_operator_token(TAN, column);
    }
    else if (!strcmp(build_buffer, "asin"))
    {
        return create_operator_token(ASIN, column);
    }
    else if (!strcmp(build_buffer, "acos"))
    {
        return create_operator_token(ACOS, column);
    }
    else if (!strcmp(build_buffer, "atan"))
    {
        return create_operator_token(ATAN, column);
    }
    else if (!strcmp(build_buffer, "sind"))
    {
        return create_operator_token(SIND, column);
    }
    else if (!strcmp(build_buffer, "cosd"))
    {
        return create_operator_token(COSD, column);
    }
    else if (!strcmp(build_buffer, "tand"))
    {
        return create_operator_token(TAND, column);
    }
    else if (!strcmp(build_buffer, "asind"))
    {
        return create_operator_token(ASIND, column);
    }
    else if (!strcmp(build_buffer, "acosd"))
    {
        return create_operator_token(ACOSD, column);
    }
    else if (!strcmp(build_buffer, "atand"))
    {
        return create_operator_token(ATAND, column);
    }
    else if (!strcmp(build_buffer, "ln"))
    {
        return create_operator_token(LN, column);
    }
    else if (!strcmp(build_buffer, "log"))
    {
        return create_operator_token(LOG, column);
    }
    else if (!strcmp(build_buffer, "abs"))
    {
        return create_operator_token(ABS, column);
    }
    else if (!strcmp(build_buffer, "fac"))
    {
        return create_operator_token(FAC, column);
    }
    else if (!strcmp(build_buffer, "pi"))
    {
        return create_number_token(PI, column);
    }
    else
    {
        // in case of no match there is an error in the input
        data->status = INVALID_INPUT_CHARACTER;

        // set index at the beginning for main lex loop error reporting
        *input_idx = column;

        return create_empty_token();
    }

}

static Token build_number_token(const char *input, unsigned int *input_idx, LexData *data)
{
    bool dot_encountered = false;

    // numbers are converted from multiple characters
    // keep a buffer of characters for conversion
    char build_buffer[BUFSIZE];
    unsigned int build_buf_idx = 0;

    // record first input character as token starting column
    unsigned int column = *input_idx;

    // the character under the passed index is assumed to be a digit
    // and is added to the buffer
    build_buffer[build_buf_idx] = input[*input_idx];

    build_buf_idx += 1;
    *input_idx += 1;

    // collect any subsequent numbers from input
    // if dot is encountered set type to float
    // stop at first non number character, excluding dot
    // report error if dot is encoutnered more than once
    bool processing_number = true;

    while (processing_number)
    {
        if (isdigit(input[*input_idx]))
        {
            // add digit to buffer
            build_buffer[build_buf_idx] = input[*input_idx];
            *input_idx += 1;

            // final spot in the array is reserved for \0
            if (build_buf_idx < BUFSIZE - 2)
                build_buf_idx += 1;
        }

        else if (input[*input_idx] == '.')
        {
            // only process a dot character once
            if (dot_encountered == false)
            {
                // add dot to buffer
                build_buffer[build_buf_idx] = '.';
                *input_idx += 1;

                // final spot in the array is reserved for \0
                if (build_buf_idx < BUFSIZE - 2)
                    build_buf_idx += 1;

                dot_encountered = true;
            }

            else
            {
                // two dots within a single operand is an error
                // that the main loop needs to handle
                data->status = MULTIPLE_DECIMAL_POINTS;
                Token res = create_empty_token();
                return res;
            }
        }

        else // not a digit or dot
        {
            processing_number = false;
        }
    }

    // set index to last processed character
    *input_idx -= 1;

    // if the final character in the buffer is a dot
    // that is an error
    if (build_buffer[build_buf_idx - 1] == '.')
    {
        data->status = NUM_ENDS_WITH_DOT;
        Token res = create_empty_token();
        return res;
    }

    // add clsoing \0 to buffer
    build_buffer[build_buf_idx] = '\0';

    // build number
    return create_number_token(atof(build_buffer), column);
}

static Token create_token(const char *input, unsigned int *input_idx, LexData *data)
{

    if (isdigit(input[*input_idx]))
    {
        return build_number_token(input, input_idx, data);
    }

    else if (islower(input[*input_idx]))
    {
        return process_text(input, input_idx, data);
    }

    else if (input[*input_idx] == '+')
    {
        return create_operator_token(ADD, *input_idx);
    }

    else if (input[*input_idx] == '-')
    {
        return create_operator_token(SUB, *input_idx);
    }

    else if (input[*input_idx] == '*')
    {
        return create_operator_token(MULT, *input_idx);
    }

    else if (input[*input_idx] == '/')
    {
        return create_operator_token(DIV, *input_idx);
    }

    else if (input[*input_idx] == '%')
    {
        return create_operator_token(MOD, *input_idx);
    }

    else if (input[*input_idx] == '^')
    {
        return create_operator_token(POW, *input_idx);
    }

    else if (input[*input_idx] == '(')
    {
        return create_parenthesis_token(LEFT, *input_idx);
    }

    else if (input[*input_idx] == ')')
    {
        return create_parenthesis_token(RIGHT, *input_idx);
    }

    else
    {
        // if no suitable operation is found
        // then there is an error and the main loop is notified
        data->status = INVALID_INPUT_CHARACTER;
        Token res = create_empty_token();
        return res;
    }
}

ResultInfo lex (const char *input, TokenList *output)
{
    LexData data = init();
    clear_tokenlist(output);

    ResultInfo res;

    // build tokens
    for (unsigned int i = 0; input[i] != '\0'; i++)
    {
        if (isspace(input[i]))
            continue;

        tokenlist_add(output, create_token(input, &i, &data));

        if (data.status != SUCCESS)
        {
            res.status = data.status;
            res.error_index = i;
            return res;
        }
    }

    res.status = SUCCESS;
    res.error_index = 0;
    return res;
}
