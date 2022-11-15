// standard library includes
#include <stdlib.h>

// project includes
#include "token.h"

// global allocation trackers
// (not thread safe)
static unsigned int list_alloc_count = 0;

Token create_empty_token(void)
{
    Token res;
    res.type = EMPTY;
    res.column = 0;

    return res;
}

Token create_number_token(double value, unsigned int column)
{
    Token res;
    res.type = NUMBER;
    res.column = column;
    res.value.number = value;

    return res;
}

Token create_operator_token(operator_type type, int column)
{
    Token res;
    res.type = OPERATOR;
    res.value.operator = type;
    res.column = column;

    return res;
}

unsigned int precedence(operator_type t)
{
    switch(t)
    {
        case ADD: return 1; break;
        case SUB: return 1; break;
        case MULT: return 2; break;
        case DIV: return 2; break;
        case MOD: return 2; break;
        case POW: return 3; break;
        case NEG: return 4; break;
        case SIN: return 4; break;
        case COS: return 4; break;
        case TAN: return 4; break;
        case ASIN: return 4; break;
        case ACOS: return 4; break;
        case ATAN: return 4; break;
        case SIND: return 4; break;
        case COSD: return 4; break;
        case TAND: return 4; break;
        case ASIND: return 4; break;
        case ACOSD: return 4; break;
        case ATAND: return 4; break;
        case LN: return 4; break;
        case LOG: return 4; break;
        case ABS: return 4; break;
        case FAC: return 4; break;
        default: return 0; break;
    }
}

bool isunary(operator_type t)
{
    if (t == SIN || t == COS || t == TAN ||
        t == ASIN || t == ACOS || t == ATAN ||
        t == SIND || t == COSD || t == TAND ||
        t == ASIND || t == ACOSD || t == ATAND ||
        t == LN || t == LOG || t == ABS || t == FAC ||
        t == NEG)
    {
        return true;
    }

    return false;
}

Token create_parenthesis_token(parenthesis_type type, int column)
{
    Token res;
    res.type = PARENTHESIS;
    res.column = column;

    if (type == LEFT)
        res.value.parenthesis = LEFT;
    else if (type == RIGHT)
        res.value.parenthesis = RIGHT;

    return res;
}

// token list functions
TokenList new_tokenlist()
{
    TokenList obj;
    obj.count = 0;
    obj.max = 8;
    obj.list = (Token *)calloc(8, sizeof(Token));
    list_alloc_count += 1;

    if (obj.list == NULL) exit(1);

    return obj;
}

void delete_tokenlist(TokenList tokenlist)
{
    free(tokenlist.list);
    list_alloc_count -= 1;
}

void clear_tokenlist(TokenList *self)
{
    self->count = 0;
}

void tokenlist_add(TokenList *self, Token value)
{
    if (self->count < self->max)
    {
        self->list[self->count] = value;
        self->count += 1;
    }

    else
    {
        self->max = self->max * 2;
        self->list = (Token *)realloc(self->list, self->max * sizeof(Token));
        if (self->list == NULL) exit(1);

        self->list[self->count] = value;
        self->count += 1;
    }
}

Token tokenlist_pop(TokenList *self)
{
    if (self->count > 0)
    {
        self->count -= 1;
        Token res = self->list[self->count];
        return res;
    }

    else
    {
        Token res;
        res.type = EMPTY;
        res.column = 0;

        return res;
    }
}

unsigned int tokenlist_allocation_count(void)
{
    return list_alloc_count;
}
