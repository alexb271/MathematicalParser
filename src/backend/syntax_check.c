// standard library includes
#include <stdbool.h>

// project includes
#include "token.h"
#include "parser.h"

// syntax check process variables
typedef struct
{
    bool operand_expected;
    bool operator_expected;
    bool left_par_allowed;
    bool sign_allowed;

    unsigned int left_par_count;
    unsigned int right_par_count;

    error_type status;
} SyntaxCheckData;

static SyntaxCheckData init()
{
    SyntaxCheckData data;
    data.operand_expected = true;
    data.operator_expected = false;
    data.left_par_allowed = true;
    data.sign_allowed = true;

    data.left_par_count = 0;
    data.right_par_count = 0;

    data.status = SUCCESS;
    return data;
}

static bool par_check(parenthesis_type t, SyntaxCheckData *data)
{
    if (t == LEFT)
    {
        data->left_par_count += 1;
        return true;
    }

    else
    {
        data->right_par_count += 1;

        if (data->right_par_count > data->left_par_count)
        {
            data->status = UNMATCHED_RIGHT_PAR;
            return false;
        }

        return true;
    }
}

static bool validate_operand_expected(Token token, SyntaxCheckData *data)
{
    if (token.type == PARENTHESIS)
    {
        if (data->left_par_allowed && token.value.parenthesis == LEFT)
        {
            data->sign_allowed = true;
            return par_check(LEFT, data);
        }

        data->status = INVALID_TOKEN;
        return false;
    }

    if (token.type == OPERATOR)
    {
        if (data->sign_allowed &&
            (token.value.operator== ADD ||
             token.value.operator== SUB))
        {
            data->sign_allowed = false;
            return true;
        }

        else if (isunary(token.value.operator))
        {
            data->sign_allowed = true;
            data->left_par_allowed = true;
            return true;
        }

        data->status = INVALID_TOKEN;
        return false;
    }

    if (token.type == NUMBER)
    {
        data->operand_expected = false;
        data->sign_allowed = true;
        data->operator_expected = true;
        return true;
    }

    data->status = INVALID_TOKEN;
    return false;
}

static bool validate_operator_expected(Token token, SyntaxCheckData *data)
{
    if (token.type == PARENTHESIS)
    {
        if (token.value.parenthesis == RIGHT)
        {
            data->left_par_allowed = false;
            return par_check(RIGHT, data);
        }

        data->status = INVALID_TOKEN;
        return false;
    }

    if (token.type == OPERATOR)
    {
        if (isunary(token.value.operator))
        {
            data->status = INVALID_TOKEN;
            return false;
        }

        data->operator_expected = false;
        data->operand_expected = true;
        data->left_par_allowed = true;
        data->sign_allowed = true;
        return true;
    }

    data->status = INVALID_TOKEN;
    return false;
}

static bool validate_token(Token token, SyntaxCheckData *data)
{
    if (data->operand_expected)
    {
        return validate_operand_expected(token, data);
    }

    else if (data->operator_expected)
    {
        return validate_operator_expected(token, data);
    }

    data->status = INVALID_TOKEN;
    return false;
}

static bool final_validate(Token token, SyntaxCheckData *data)
{
    // check validity of final token
    // check for mismatched parentheses
    // final token must be right parenthesis or operand
    if (data->right_par_count < data->left_par_count)
    {
        data->status = UNMATCHED_LEFT_PAR;
        return false;
    }

    if (token.type == NUMBER)
    {
        return true;
    }

    else if (token.type == PARENTHESIS && token.value.parenthesis == RIGHT)
    {
        return true;
    }

    data->status = INVALID_TOKEN;
    return false;
}

static bool validate(const TokenList t_list, SyntaxCheckData *data, ResultInfo *res)
{
    for (unsigned int i = 0; i < t_list.count; i++)
    {
        if (!validate_token(t_list.list[i], data))
        {
           res->status = data->status;
           res->error_index = t_list.list[i].column;
           return false;
        }
    }

    // check validity of final token
    // final token must be right parenthesis or operand
    // also check for mismatched parentheses
    if (t_list.count > 0 && !final_validate(t_list.list[t_list.count - 1], data))
    {
        res->status = data->status;
        res->error_index = t_list.list[t_list.count - 1].column;
        return false;
    }

    return true;
}

ResultInfo syntax_check(const TokenList tokens)
{
    ResultInfo res;

    SyntaxCheckData data = init();

    if (!validate(tokens, &data, &res))
    {
        return res;
    }

    res.status = SUCCESS;
    res.error_index = 0;
    return res;
}
