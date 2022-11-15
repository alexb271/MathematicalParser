// standard library includes
#include <stdbool.h>
#include <stddef.h>

// project includes
#include "token.h"
#include "parser.h"

// convert process variables
typedef struct
{
    // sign is expected if the current token is + or -
    // and the previous token was an operator or left parenthesis
    bool sign_expected;
} ConvertData;

static ConvertData init(void)
{
    ConvertData data;
    data.sign_expected = true;
    return data;
}

static void process_parenthesis (
        TokenList *input,
        TokenList *output,
        TokenList *stack,
        unsigned int idx,
        ConvertData *data)
{
    // if left parenthesis, push it on the stack
    if (input->list[idx].value.parenthesis == LEFT)
    {
        tokenlist_add(stack, input->list[idx]);
        data->sign_expected = true;
    }

    // if right parenthesis, put operators into output
    // until left parenthesis is found
    else if (input->list[idx].value.parenthesis == RIGHT)
    {
        Token temp = create_empty_token();
        bool left_par_found = false;
        while (!left_par_found)
        {
            temp = tokenlist_pop(stack);
            if (temp.type == PARENTHESIS && temp.value.parenthesis == LEFT)
            {
                left_par_found = true;
            }

            else
            {
                tokenlist_add(output, temp);
            }
        }
        data->sign_expected = false;
    }
}

static void process_operator (
        TokenList *input,
        TokenList *output,
        TokenList *stack,
        unsigned int idx)
{
    // if the stack is empty, simply push to the stack
    if (stack->count == 0)
    {
        tokenlist_add(stack, input->list[idx]);
        return;
    }

    // else the action depends on the most recent item in the stack
    Token t = stack->list[stack->count - 1];

    // if the top of the stack is left parenthesis, push operator on the stack
    if (t.type == PARENTHESIS && t.value.parenthesis == LEFT)
    {
        tokenlist_add(stack, input->list[idx]);
    }

    // if the top of the stack has lower precedence, push operator on the stack
    else if (precedence(t.value.operator) <
            precedence(input->list[idx].value.operator))
    {
        tokenlist_add(stack, input->list[idx]);
    }

    // if the top of the stack has higher precedence,
    // move it from the stack to the output
    // and check the top of the stack again
    else if (precedence(t.value.operator) >
             precedence(input->list[idx].value.operator))
    {
        tokenlist_add(output, tokenlist_pop(stack));

        process_operator(input, output, stack, idx);
    }

    // same precedence
    //
    // unary operators are all right to left associative
    // so push them on the stack
    //
    // binary operators are all left to right associative
    // so move the top of the stack to the output,
    // and push the current operator on the stack
    else if (isunary(input->list[idx].value.operator))
    {
        tokenlist_add(stack, input->list[idx]);
    }
    else
    {
        tokenlist_add(output, tokenlist_pop(stack));
        tokenlist_add(stack, input->list[idx]);
    }
}

static void process (
        TokenList *input,
        TokenList *output,
        TokenList *stack,
        unsigned int idx,
        ConvertData *data)
{
    // if operand, add directly to output
    if (input->list[idx].type == NUMBER)
    {
        tokenlist_add(output, input->list[idx]);
        data->sign_expected = false;
    }

    // parenthesis specific actions
    else if (input->list[idx].type == PARENTHESIS)
    {
        process_parenthesis(input, output, stack, idx, data);
    }

    // operator specific actions
    else if (input->list[idx].type == OPERATOR)
    {
        // merge sign tokens into their operand tokens
        if (data->sign_expected &&
            (input->list[idx].value.operator== ADD ||
            input->list[idx].value.operator== SUB))
        {
            //if sign is + simply skip it
            if (input->list[idx].value.operator== ADD)
                return;

            // if sign is - and the next token is a single operand,
            // invert the operand and skip the - token

            // if the next token is a left parenthesis or unary operator,
            // then push a negative token to the stack
            else if (input->list[idx].value.operator== SUB)
            {
                if (input->list[idx + 1].type == NUMBER)
                    input->list[idx + 1].value.number *= -1;

                else if ((input->list[idx + 1].type == PARENTHESIS &&
                         input->list[idx + 1].value.parenthesis == LEFT)
                         ||
                         (input->list[idx + 1].type == OPERATOR &&
                         isunary(input->list[idx + 1].value.operator)))
                {
                    tokenlist_add(stack, create_operator_token(NEG, input->list[idx].column));
                }
            }
        }

        else
        {
            process_operator(input, output, stack, idx);
            data->sign_expected = true;
        }
    }
}

ResultInfo convert(const char *input_string, TokenList *tokens)
{
    ResultInfo res;
    TokenList buffer = new_tokenlist();

    // build tokens from input string
    res = lex(input_string, &buffer);
    if (res.status != SUCCESS)
    {
        delete_tokenlist(buffer);
        return res;
    }

    // check validity of constructed tokens
    res = syntax_check(buffer);
    if (res.status != SUCCESS)
    {
        delete_tokenlist(buffer);
        return res;
    }

    // convert tokens to postfix notation and handle operand signs
    // overwrite input list entirely
    clear_tokenlist(tokens);
    TokenList stack = new_tokenlist();
    ConvertData data = init();

    for (unsigned int i = 0; i < buffer.count; i++)
    {
        process(&buffer, tokens, &stack, i, &data);
    }

    // print all remaining operators from the stack
    while (stack.count > 0)
    {
        tokenlist_add(tokens, tokenlist_pop(&stack));
    }

    // free data structures
    delete_tokenlist(stack);
    delete_tokenlist(buffer);

    res.status = SUCCESS;
    res.error_index = 0;
    return res;
}
