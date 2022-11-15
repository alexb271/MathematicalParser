// standard library includes
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// project includes
#include "token.h"

static bool print_token_empty (Token token)
{
    if (token.type == EMPTY)
    {
        printf("EMPTY");
        return true;
    }

    return false;
}

static bool print_token_number (Token token)
{
    if (token.type == NUMBER)
    {
        // count length of output with
        // snprintf while writing 0 characters to a null pointer
        // (not writing anything)
        int len = snprintf(NULL, 0, "%f", token.value.number) + 1;

        // print to a buffer first
        char *buf = malloc(len);
        if (buf == NULL) exit(1);
        sprintf(buf, "%f", token.value.number);

        // count the integer part until the dot
        int i = 0;
        while (i < len && buf[i] != '.')
        {
            i++;
        }

        // if integer part is longer than 10 character
        // print in scientific notation
        if (i > 10)
        {
            printf("%.10e", token.value.number);
            free(buf);
            return true;
        }

        // remove trailing zeros
        // since the length above is exact, len - 1 is the original '\0'
        // start at len - 2, and replace occurances of 0 with '\0'
        for (int i = len - 2; i >= 0; i--)
        {
            if (buf[i] == '0')
                buf[i] = '\0';
            else
            {
                if (buf[i] == '.')
                    buf[i] = '\0';
                break;
            }
        }

        // handle -0
        if (buf[0] == '-' && buf[1] == '0' && buf[2] == '\0')
        {
            printf("0");
        }
        else
        {
            printf("%s", buf);
        }

        free(buf);
        return true;
    }

    return false;
}

static bool print_token_operator (Token token)
{
    if (token.type == OPERATOR)
    {
        operator_type t = token.value.operator;

        if (t == ADD)
            printf("+");
        else if (t == SUB)
            printf("-");
        else if (t == MULT)
            printf("*");
        else if (t == DIV)
            printf("/");
        else if (t == MOD)
            printf("%%");
        else if (t == POW)
            printf("^");
        else if (t == NEG)
            printf("neg");
        else if (t == SIN)
            printf("sin");
        else if (t == COS)
            printf("cos");
        else if (t == TAN)
            printf("tan");
        else if (t == ASIN)
            printf("asin");
        else if (t == ACOS)
            printf("acos");
        else if (t == ATAN)
            printf("atan");
        else if (t == SIND)
            printf("sind");
        else if (t == COSD)
            printf("cosd");
        else if (t == TAND)
            printf("tand");
        else if (t == ASIND)
            printf("asind");
        else if (t == ACOSD)
            printf("acosd");
        else if (t == ATAND)
            printf("atand");
        else if (t == LN)
            printf("ln");
        else if (t == LOG)
            printf("log");

        return true;
    }

    return false;
}

static bool print_token_parenthesis (Token token)
{
    if (token.type == PARENTHESIS)
    {
        parenthesis_type t = token.value.parenthesis;

        if (t == LEFT)
            printf("(");
        else if (t == RIGHT)
            printf(")");

        return true;
    }

    return false;
}

void print_token(Token token)
{
    if (print_token_empty(token)) {}
    else if (print_token_number(token)) {}
    else if (print_token_operator(token)) {}
    else if (print_token_parenthesis(token)) {}
}

void print_tokenlist(TokenList t_list)
{
    if (t_list.count == 0)
        return;

    for (unsigned int i = 0; i < t_list.count; i++)
    {
        print_token(t_list.list[i]);
        if (i < t_list.count - 1)
            putchar(' ');
    }
    putchar('\n');
}
