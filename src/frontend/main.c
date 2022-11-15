// standard library includes
#include <stdio.h>
#include <string.h>

// project includes
#include "tokenprint.h"
#include "parser.h"

#define BUFSIZE 1024

static void print_result_error(ResultInfo resinfo, const char *input_string);
static void interactive_mode(void);

int main(int argc, const char **argv)
{
    if (argc == 1)
    {
        interactive_mode();
        return 0;
    }

    else if (argc == 2)
    {
        if (!strcmp(argv[1], "-h"))
        {
            printf( "usage: %s [-p] [expression]\n\n", *argv);
            printf( "%s",
                    "default           interactive mode\n"
                    "expression        calculate expression\n"
                    "-p  expression    print expression in postfix notation\n"
                    "\nOperators: + - * / % ^\n"
                    "Functions: sin, cos, tan, ln, log, abs, fac\n"
                    "For trig functions prepend 'a' for arcus and append 'd' for degree.\n"
                    "Use 'pi' for an accurate value of the constant.\n"
                  );

            return 0;
        }

        Token result = create_empty_token();
        ResultInfo parse_res = parse(argv[1], &result);

        if (parse_res.status != SUCCESS)
        {
            print_result_error(parse_res, argv[1]);
        }

        else
        {
            print_token(result);
            putchar('\n');
        }

        return 0;
    }

    else if (argc == 3 && !strcmp(argv[1], "-p"))
    {
        TokenList t_list = new_tokenlist();
        ResultInfo conv_res = convert(argv[2], &t_list);

        if (conv_res.status != SUCCESS)
        {
            print_result_error(conv_res, argv[2]);
        }

        else
        {
            print_tokenlist(t_list);
        }

        delete_tokenlist(t_list);
        return 0;
    }

    else
    {
        printf( "usage: %s [-p] [expression]\n", *argv);
        return 0;
    }
}

static void print_result_error(ResultInfo resinfo, const char *input_string)
{
    if (resinfo.status == SUCCESS)
        return;

    // special case when the error marker goes at the end of the input string
    if (resinfo.status == UNMATCHED_LEFT_PAR)
    {
        // print input string and error indicator
        printf("%s\n", input_string);
        for (unsigned int i = 0; i < strlen(input_string); i++)
            putchar(' ');
        printf("^\n");

        // error message
        printf("SyntaxError: ')' expected\n");
        return;
    }

    // print input string and error indicator
    printf("%s\n", input_string);
    for (unsigned int i = 0; i < resinfo.error_index; i++)
        putchar(' ');
    printf("^\n");

    // print error message
    if (resinfo.status == MULTIPLE_DECIMAL_POINTS)
        printf("SyntaxError: Multiple decimal points in number\n");
    else if (resinfo.status == NUM_ENDS_WITH_DOT)
        printf("SyntaxError: Number ending with decimal point\n");
    else if (resinfo.status == INVALID_INPUT_CHARACTER)
        printf("SyntaxError: Invalid input\n");
    else if (resinfo.status == INVALID_TOKEN)
        printf("SyntaxError: Invalid token\n");
    else if (resinfo.status == UNMATCHED_RIGHT_PAR)
        printf("SyntaxError: Unmatched right parentheses\n");
    else if (resinfo.status == ZERO_DIVISON)
        printf("MathError: Divison by zero\n");
    else if (resinfo.status == NEGATIVE_FRACTIONAL_EXPONENT)
        printf("ParseError: Negative number with fractional exponent not supported\n");
    else if (resinfo.status == ZERO_NEGATIVE_EXPONENT)
        printf("MathError: Zero with negative exponent\n");
    else if (resinfo.status == TANGENT_UNDEFINED)
        printf("MathError: Tangent of argument is undefined\n");
    else if (resinfo.status == ARCUS_OUT_OF_RANGE)
        printf("MathError: Arcus function argument out of range\n");
    else if (resinfo.status == LOG_OUT_OF_RANGE)
        printf("MathError: Logarithm function argument out of range\n");
    else if (resinfo.status == FAC_INPUT_NOT_INT)
        printf("MathError: Factorial input must be an integer\n");
}

static void interactive_mode(void)
{
    // input buffer
    char buffer[BUFSIZE];

    // print greeter message
    printf( "%s",
            "Mathematical parser v1.0\n"
            "Input Q to quit\n");

    while(1)
    {
        // input handling variables
        unsigned int buf_idx = 0;
        signed char input;

        // parse variables
        ResultInfo parse_res;
        Token result = create_empty_token();

        // get one line of input
        printf(">>> ");
        do
        {
            input = getchar();

            // handle Ctrl+D in empty line
            if (input == EOF)
            {
                putchar('\n');
                return;
            }

            buffer[buf_idx] = input;
            if (buf_idx < BUFSIZE -1)
                buf_idx += 1;
        } while (input != '\n');

        // handle closing null character on input
        // final character is \n, not needed for parse
        buffer[buf_idx - 1] = '\0';

        // handle exit
        if (!strcmp(buffer, "Q") || !strcmp(buffer, "q"))
            break;

        // parse the string and print error or result
        if (buf_idx > 0)
            parse_res = parse(buffer, &result);

        if (parse_res.status != SUCCESS)
        {
            print_result_error(parse_res, buffer);
        }
        else
        {
            print_token(result);
            putchar('\n');
        }
    }
}
