// standard library includes
#include <math.h>
#include <stddef.h>
#include <limits.h>

// project includes
#include "token.h"
#include "parser.h"

#define PI 3.14159265358979323846264338327950288

static const double deg_to_rad = PI / 180;
static const double rad_to_deg = 180 / PI;

// parse process variables
typedef struct
{
    error_type status;
} ParseData;

static ParseData init()
{
    ParseData data;
    data.status = SUCCESS;
    return data;
}

static void addition(TokenList *stack)
{
    Token right_token = tokenlist_pop(stack);
    Token left_token = tokenlist_pop(stack);

    double res = left_token.value.number + right_token.value.number;
    tokenlist_add(stack, create_number_token(res, 0));
}

static void subtraction(TokenList *stack)
{
    Token right_token = tokenlist_pop(stack);
    Token left_token = tokenlist_pop(stack);

    double res = left_token.value.number - right_token.value.number;
    tokenlist_add(stack, create_number_token(res, 0));
}

static void multiplication(TokenList *stack)
{
    Token right_token = tokenlist_pop(stack);
    Token left_token = tokenlist_pop(stack);

    double res = left_token.value.number * right_token.value.number;
    tokenlist_add(stack, create_number_token(res, 0));
}

static void division(TokenList *stack, ParseData *data)
{
    Token right_token = tokenlist_pop(stack);
    Token left_token = tokenlist_pop(stack);

    // check for divison with zero on right operand
    if (right_token.value.number == 0)
    {
        data->status = ZERO_DIVISON;
    }

    else
    {
        double res = left_token.value.number / right_token.value.number;
        tokenlist_add(stack, create_number_token(res, 0));
    }
}

static void power(TokenList *stack, ParseData *data)
{
    Token right_token = tokenlist_pop(stack);
    Token left_token = tokenlist_pop(stack);

    // handle negative number with fractional exponent
    if (left_token.value.number < 0 &&
        trunc(right_token.value.number) != right_token.value.number)
    {
        data->status = NEGATIVE_FRACTIONAL_EXPONENT;
    }

    // handle zero with negative exponent
    else if (left_token.value.number == 0 &&
             right_token.value.number < 0)
    {
        data->status = ZERO_NEGATIVE_EXPONENT;
    }

    else
    {
        double res = pow(left_token.value.number, right_token.value.number);
        tokenlist_add(stack, create_number_token(res, 0));
    }
}

static void modulo(TokenList *stack)
{
    Token right_token = tokenlist_pop(stack);
    Token left_token = tokenlist_pop(stack);

    double res = fmod(left_token.value.number, right_token.value.number);
    tokenlist_add(stack, create_number_token(res, 0));
}

static void negative_inversion(TokenList *stack)
{
    double operand = tokenlist_pop(stack).value.number;
    double res = -1.0 * operand;
    tokenlist_add(stack, create_number_token(res, 0));
}

static void sine(TokenList *stack)
{
    double operand = tokenlist_pop(stack).value.number;
    double res = sin(operand);
    tokenlist_add(stack, create_number_token(res, 0));
}

static void cosine(TokenList *stack)
{
    double operand = tokenlist_pop(stack).value.number;
    double res = cos(operand);
    tokenlist_add(stack, create_number_token(res, 0));
}

static void tangent(TokenList *stack, ParseData *data)
{
    double operand = tokenlist_pop(stack).value.number;
    if (fabs(fmod(operand, PI)) == PI / 2)
    {
        data->status = TANGENT_UNDEFINED;
    }

    else
    {
        double res = tan(operand);
        tokenlist_add(stack, create_number_token(res, 0));
    }
}

static void arcus_sine(TokenList *stack, ParseData *data)
{
    double operand = tokenlist_pop(stack).value.number;

    if (operand > 1 || operand < -1)
    {
        data->status = ARCUS_OUT_OF_RANGE;
    }

    else
    {
        double res = asin(operand);
        tokenlist_add(stack, create_number_token(res, 0));
    }
}

static void arcus_cosine(TokenList *stack, ParseData *data)
{
    double operand = tokenlist_pop(stack).value.number;

    if (operand > 1 || operand < -1)
    {
        data->status = ARCUS_OUT_OF_RANGE;
    }

    else
    {
        double res = acos(operand);
        tokenlist_add(stack, create_number_token(res, 0));
    }
}

static void arcus_tangent(TokenList *stack)
{
    double operand = tokenlist_pop(stack).value.number;
    double res = atan(operand);
    tokenlist_add(stack, create_number_token(res, 0));
}

static void sine_deg(TokenList *stack)
{
    double operand = tokenlist_pop(stack).value.number;
    double res = sin(operand * deg_to_rad);
    tokenlist_add(stack, create_number_token(res, 0));
}

static void cosine_deg(TokenList *stack)
{
    double operand = tokenlist_pop(stack).value.number;
    double res = cos(operand * deg_to_rad);
    tokenlist_add(stack, create_number_token(res, 0));
}

static void tangent_deg(TokenList *stack, ParseData *data)
{
    double operand = tokenlist_pop(stack).value.number;
    if (fabs(fmod(operand, 180)) == 90)
    {
        data->status = TANGENT_UNDEFINED;
    }

    else
    {
        double res = tan(operand * deg_to_rad);
        tokenlist_add(stack, create_number_token(res, 0));
    }
}

static void arcus_sine_deg(TokenList *stack, ParseData *data)
{
    double operand = tokenlist_pop(stack).value.number;

    if (operand > 1 || operand < -1)
    {
        data->status = ARCUS_OUT_OF_RANGE;
    }

    else
    {
        double res = asin(operand);
        tokenlist_add(stack, create_number_token(res * rad_to_deg, 0));
    }
}

static void arcus_cosine_deg(TokenList *stack, ParseData *data)
{
    double operand = tokenlist_pop(stack).value.number;

    if (operand > 1 || operand < -1)
    {
        data->status = ARCUS_OUT_OF_RANGE;
    }

    else
    {
        double res = acos(operand);
        tokenlist_add(stack, create_number_token(res * rad_to_deg, 0));
    }
}

static void arcus_tangent_deg(TokenList *stack)
{
    double operand = tokenlist_pop(stack).value.number;
    double res = atan(operand);
    tokenlist_add(stack, create_number_token(res * rad_to_deg, 0));
}

static void log_nat(TokenList *stack, ParseData *data)
{
    double operand = tokenlist_pop(stack).value.number;
    if (operand <= 0)
    {
        data->status = LOG_OUT_OF_RANGE;
    }

    else
    {
        double res = log(operand);
        tokenlist_add(stack, create_number_token(res, 0));
    }
}

static void log_dec(TokenList *stack, ParseData *data)
{
    double operand = tokenlist_pop(stack).value.number;
    if (operand <= 0)
    {
        data->status = LOG_OUT_OF_RANGE;
    }

    else
    {
        double res = log10(operand);
        tokenlist_add(stack, create_number_token(res, 0));
    }
}

static void absolute_value(TokenList *stack)
{
    double operand = tokenlist_pop(stack).value.number;
    double res = fabs(operand);
    tokenlist_add(stack, create_number_token(res, 0));
}

static void factorial(TokenList *stack, ParseData *data)
{
    double operand = tokenlist_pop(stack).value.number;

    if (trunc(operand) != operand)
    {
        data->status = FAC_INPUT_NOT_INT;
    }

    else
    {
        if (operand == 0)
        {
            tokenlist_add(stack, create_number_token(1, 0));
        }

        else
        {
            double res;
            if (operand > 0)
                res = 1;
            else
                res = -1;

            for (int i = 1; i <= fabs(operand); i++)
            {
                res *= i;
            }

            tokenlist_add(stack, create_number_token(res, 0));
        }
    }
}

static void process(Token *token, TokenList *stack, ParseData *data)
{
    // if token is operand, push it to the stack
    if (token->type == NUMBER)
    {
        tokenlist_add(stack, *token);
    }

    // if token is operator, perform the corresponding operation on the stack
    else if (token->type == OPERATOR)
    {
        if (token->value.operator== ADD)
            addition(stack);
        else if (token->value.operator== SUB)
            subtraction(stack);
        else if (token->value.operator== MULT)
            multiplication(stack);
        else if (token->value.operator== DIV)
            division(stack, data);
        else if (token->value.operator== MOD)
            modulo(stack);
        else if (token->value.operator== POW)
            power(stack, data);
        else if (token->value.operator== NEG)
            negative_inversion(stack);
        else if (token->value.operator== SIN)
            sine(stack);
        else if (token->value.operator== COS)
            cosine(stack);
        else if (token->value.operator== TAN)
            tangent(stack, data);
        else if (token->value.operator== ASIN)
            arcus_sine(stack, data);
        else if (token->value.operator== ACOS)
            arcus_cosine(stack, data);
        else if (token->value.operator== ATAN)
            arcus_tangent(stack);
        else if (token->value.operator== SIND)
            sine_deg(stack);
        else if (token->value.operator== COSD)
            cosine_deg(stack);
        else if (token->value.operator== TAND)
            tangent_deg(stack, data);
        else if (token->value.operator== ASIND)
            arcus_sine_deg(stack, data);
        else if (token->value.operator== ACOSD)
            arcus_cosine_deg(stack, data);
        else if (token->value.operator== ATAND)
            arcus_tangent_deg(stack);
        else if (token->value.operator== LN)
            log_nat(stack, data);
        else if (token->value.operator== LOG)
            log_dec(stack, data);
        else if (token->value.operator== ABS)
            absolute_value(stack);
        else if (token->value.operator== FAC)
            factorial(stack, data);
    }
}

ResultInfo parse(const char *input_string, Token *result)
{
    ResultInfo res;
    ParseData data = init();
    TokenList buffer = new_tokenlist();

    res = convert(input_string, &buffer);
    if (res.status != SUCCESS)
    {
        delete_tokenlist(buffer);
        return res;
    }

    TokenList stack = new_tokenlist();
    for (unsigned int i = 0; i < buffer.count; i++)
    {
        process(&buffer.list[i], &stack, &data);

        if (data.status != SUCCESS)
        {
            res.status = data.status;
            res.error_index = buffer.list[i].column;

            delete_tokenlist(buffer);
            delete_tokenlist(stack);
            return res;
        }
    }

    if (stack.count > 0)
    {
        *result = tokenlist_pop(&stack);
    }

    else
    {
        *result = create_number_token(0, 0);
    }

    delete_tokenlist(buffer);
    delete_tokenlist(stack);

    res.status = SUCCESS;
    res.error_index = 0;
    return res;
}
