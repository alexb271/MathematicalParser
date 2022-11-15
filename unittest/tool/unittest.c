// standard library includes
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

// project includes
#include "parser.h"
#include "unittest.h"

static const char *test_domain_name;
static unsigned int test_count;
static unsigned int successful_test_count;

void begin_test_domain(const char *name)
{
    test_domain_name = name;
    test_count = 0;
    successful_test_count = 0;
}

void conclude_test_domain(void)
{
    if (test_count == successful_test_count)
        printf("%s: All %d tests passed\n",
                test_domain_name,
                test_count);
    else
        printf("%s: %d tests failed\n",
                test_domain_name, test_count - successful_test_count);
    putchar('\n');
}

static void print_error_name(error_type input)
{
    if (input == MULTIPLE_DECIMAL_POINTS)
        printf("MULTIPLE_DECIMAL_POINTS");
    else if (input == NUM_ENDS_WITH_DOT)
        printf("NUM_ENDS_WITH_DOT");
    else if (input == INVALID_INPUT_CHARACTER)
        printf("INVALID_INPUT_CHARACTER");
    else if (input == INVALID_TOKEN)
        printf("INVALID_TOKEN");
    else if (input == UNMATCHED_LEFT_PAR)
        printf("UNMATCHED_LEFT_PAR");
    else if (input == UNMATCHED_RIGHT_PAR)
        printf("UNMATCHED_RIGHT_PAR");
    else if (input == ZERO_DIVISON)
        printf("ZERO_DIVISON");
    else if (input == NEGATIVE_FRACTIONAL_EXPONENT)
        printf("NEGATIVE_FRACTIONAL_EXPONENT");
    else if (input == ZERO_NEGATIVE_EXPONENT)
        printf("ZERO_NEGATIVE_EXPONENT");
    else if (input == TANGENT_UNDEFINED)
        printf("TANGENT_UNDEFINED");
    else if (input == ARCUS_OUT_OF_RANGE)
        printf("ARCUS_OUT_OF_RANGE");
    else if (input == FAC_INPUT_NOT_INT)
        printf("FAC_INPUT_NOT_INT");
    else if (input == SUCCESS)
        printf("SUCCESS");
}

static bool tokens_equal(Token a, Token b)
{
    if (a.type == b.type && a.column == b.column)
    {
        if (a.type == NUMBER)
        {
            if (a.value.number == b.value.number)
                    return true;
        }

        else if (a.type == OPERATOR)
        {
            if (a.value.operator== b.value.operator)
                return true;
        }

        else if (a.type == PARENTHESIS)
        {
            if (a.value.parenthesis == b.value.parenthesis)
                return true;
        }
    }
    return false;
}

static bool tokenlists_equal(TokenList a, TokenList b, unsigned int *err_idx)
{
    if (a.count == b.count)
    {
        bool equal = true;
        for (unsigned int i = 0; i < a.count; i++)
        {
            if (!tokens_equal(a.list[i], b.list[i]))
            {
                equal = false;
                *err_idx = i;
                break;
            }
        }
        return equal;
    }
    return false;
}

void assert_tokenlists_equal(TokenList expected, TokenList result)
{
    test_count += 1;

    if (expected.count != result.count)
    {
        printf("%s test #%d failed\n", test_domain_name, test_count);
        printf("Token lists have different length.\n");
        printf("Expected length: %d\n", expected.count);
        printf("Result length: %d\n", result.count);
        putchar('\n');

        return;

    }

    unsigned int idx;

    if (tokenlists_equal(expected, result, &idx))
        successful_test_count += 1;

    else
    {
        printf("%s test #%d failed\n", test_domain_name, test_count);
        printf("Token lists differ at index: %d\n", idx);
        putchar('\n');
    }
}

void assert_error(ResultInfo input, error_type errtype, unsigned int index)
{
    test_count += 1;

    if (input.status != errtype || input.error_index != index)
    {
        printf( "%s test #%d failed\n", test_domain_name, test_count);

        printf("Expected: "); print_error_name(errtype);
        printf(" at index: %d\n", index);

        printf("Result: "); print_error_name(input.status);
        printf(" at index: %d\n", input.error_index);

        putchar('\n');
    }

    else
        successful_test_count += 1;
}

void assert_success(ResultInfo input)
{
    assert_error(input, SUCCESS, 0);
}

void assert_parse_result(const char *input, double expected_result)
{
    test_count += 1;

    ResultInfo res;
    Token output = create_empty_token();

    res = parse(input, &output);
    if (res.status == SUCCESS)
    {
        if (fabs(output.value.number - expected_result) < 0.000001)
        {
            successful_test_count += 1;
        }

        else
        {
            printf( "%s test #%d failed\n", test_domain_name, test_count);
            printf("Expected: %f\n", expected_result);
            printf("Result  : %f\n", output.value.number);
            putchar('\n');
        }
    }

    else
    {
        printf( "%s test #%d failed\n", test_domain_name, test_count);
        printf("Unseccesful parse\n");
        putchar('\n');
    }
}

void assert_zero_allocations(void)
{
    test_count += 1;
    unsigned int list_alloc_count = tokenlist_allocation_count();
    if (list_alloc_count != 0)
    {
        printf( "%s test #%d failed\n", test_domain_name, test_count);
        printf("Number of allocated token lists: %d\n", list_alloc_count);
        putchar('\n');
    }

    else
    {
        successful_test_count += 1;
    }
}
