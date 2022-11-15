// project includes
#include "unittest.h"
#include "parser.h"

static void lexer_test(void)
{
    begin_test_domain("Lexer");

    TokenList subject = new_tokenlist();

    assert_error(lex("(3 + 17) * 2.57.7 - 8", &subject),
                 MULTIPLE_DECIMAL_POINTS, 15);

    assert_error(lex("5 - 9.7.1", &subject),
                 MULTIPLE_DECIMAL_POINTS, 7);

    assert_error(lex("(3 + 17) * 2.57 - 8.", &subject),
                 NUM_ENDS_WITH_DOT, 19);

    assert_error(lex("(3 + 17) * 2.57 - 8. + 2", &subject),
                 NUM_ENDS_WITH_DOT, 19);

    assert_error(lex("var + (3 + 17) * 2.57 - 8 + 2", &subject),
                 INVALID_INPUT_CHARACTER, 0);

    assert_error(lex("(3 + sinc) * 2.57 - 8 + 2", &subject),
                 INVALID_INPUT_CHARACTER, 5);

    assert_error(lex("(3 + 17) * 2.57 - 8 + var", &subject),
                 INVALID_INPUT_CHARACTER, 22);

    assert_error(lex("(3 + 17) * 2.A57 - 8 + var", &subject),
                 NUM_ENDS_WITH_DOT, 12);

    assert_error(lex("(3 + 17) * 2.9A57 - 8 + var", &subject),
                 INVALID_INPUT_CHARACTER, 14);

    assert_error(lex(".1 + 3", &subject),
                 INVALID_INPUT_CHARACTER, 0);


    // manual result verification
    TokenList expected = new_tokenlist();
    assert_success(lex("(32+3.14) - 42 % 8 * 5.0 / 2 ^ 0.5", &subject));
    tokenlist_add(&expected, create_parenthesis_token(LEFT, 0));
    tokenlist_add(&expected, create_number_token(32, 1));
    tokenlist_add(&expected, create_operator_token(ADD, 3));
    tokenlist_add(&expected, create_number_token(3.14, 4));
    tokenlist_add(&expected, create_parenthesis_token(RIGHT, 8));
    tokenlist_add(&expected, create_operator_token(SUB, 10));
    tokenlist_add(&expected, create_number_token(42, 12));
    tokenlist_add(&expected, create_operator_token(MOD, 15));
    tokenlist_add(&expected, create_number_token(8, 17));
    tokenlist_add(&expected, create_operator_token(MULT, 19));
    tokenlist_add(&expected, create_number_token(5.0, 21));
    tokenlist_add(&expected, create_operator_token(DIV, 25));
    tokenlist_add(&expected, create_number_token(2, 27));
    tokenlist_add(&expected, create_operator_token(POW, 29));
    tokenlist_add(&expected, create_number_token(0.5, 31));
    assert_tokenlists_equal(expected, subject);

    assert_success(lex("sin cos tan asin acos atan "
                       "sind cosd tand asind acosd atand ln log pi", &subject));
    clear_tokenlist(&expected);
    tokenlist_add(&expected, create_operator_token(SIN, 0));
    tokenlist_add(&expected, create_operator_token(COS, 4));
    tokenlist_add(&expected, create_operator_token(TAN, 8));
    tokenlist_add(&expected, create_operator_token(ASIN, 12));
    tokenlist_add(&expected, create_operator_token(ACOS, 17));
    tokenlist_add(&expected, create_operator_token(ATAN, 22));
    tokenlist_add(&expected, create_operator_token(SIND, 27));
    tokenlist_add(&expected, create_operator_token(COSD, 32));
    tokenlist_add(&expected, create_operator_token(TAND, 37));
    tokenlist_add(&expected, create_operator_token(ASIND, 42));
    tokenlist_add(&expected, create_operator_token(ACOSD, 48));
    tokenlist_add(&expected, create_operator_token(ATAND, 54));
    tokenlist_add(&expected, create_operator_token(LN, 60));
    tokenlist_add(&expected, create_operator_token(LOG, 63));
    tokenlist_add(&expected, create_number_token(
                3.14159265358979323846264338327950288, 67));
    assert_tokenlists_equal(expected, subject);

    // test for zero length input
    assert_success(lex("", &subject));
    clear_tokenlist(&expected);
    assert_tokenlists_equal(expected, subject);

    // release resources and conclude
    delete_tokenlist(expected);
    delete_tokenlist(subject);

    assert_zero_allocations();
    conclude_test_domain();
}

static void syntax_check_test(void)
{
    begin_test_domain("Syntax");
    TokenList subject = new_tokenlist();

    lex(")3+5", &subject);
    assert_error(syntax_check(subject), INVALID_TOKEN, 0);

    lex("3)+5", &subject);
    assert_error(syntax_check(subject), UNMATCHED_RIGHT_PAR, 1);

    lex("3+5)", &subject);
    assert_error(syntax_check(subject), UNMATCHED_RIGHT_PAR, 3);

    lex("(3+5 -1", &subject);
    assert_error(syntax_check(subject), UNMATCHED_LEFT_PAR, 6);

    lex("(3+5) -1*2)", &subject);
    assert_error(syntax_check(subject), UNMATCHED_RIGHT_PAR, 10);

    lex("(3+5) -(1*7%2", &subject);
    assert_error(syntax_check(subject), UNMATCHED_LEFT_PAR, 12);

    lex("((9-4.12) * 62/3", &subject);
    assert_error(syntax_check(subject), UNMATCHED_LEFT_PAR, 15);

    lex("((9-4.12) * 62/3))", &subject);
    assert_error(syntax_check(subject), UNMATCHED_RIGHT_PAR, 17);

    lex("*2 - 7", &subject);
    assert_error(syntax_check(subject), INVALID_TOKEN, 0);

    lex("2 - *7", &subject);
    assert_error(syntax_check(subject), INVALID_TOKEN, 4);

    lex("2 - 7+", &subject);
    assert_error(syntax_check(subject), INVALID_TOKEN, 5);

    lex("2 -(* 7+3)", &subject);
    assert_error(syntax_check(subject), INVALID_TOKEN, 4);

    lex("2+++7", &subject);
    assert_error(syntax_check(subject), INVALID_TOKEN, 3);

    lex("2^^ 7", &subject);
    assert_error(syntax_check(subject), INVALID_TOKEN, 2);

    lex("((2)*) - 7", &subject);
    assert_error(syntax_check(subject), INVALID_TOKEN, 5);

    lex("5sin", &subject);
    assert_error(syntax_check(subject), INVALID_TOKEN, 1);

    lex("5 sin 1", &subject);
    assert_error(syntax_check(subject), INVALID_TOKEN, 2);

    lex("(3+2) cos", &subject);
    assert_error(syntax_check(subject), INVALID_TOKEN, 6);

    lex("tan", &subject);
    assert_error(syntax_check(subject), INVALID_TOKEN, 0);

    lex("(tan) 5", &subject);
    assert_error(syntax_check(subject), INVALID_TOKEN, 4);

    lex("(((8+9)*10)^2)-(1+2-3*4/5^6%2-1)", &subject);
    assert_success(syntax_check(subject));

    lex("(((sin8+cos(9))*tan10)^asin2)-(acosd1+ln2-log3*tand4/5^6%2-1)", &subject);
    assert_success(syntax_check(subject));

    // test for zero length input
    lex("", &subject);
    assert_success(syntax_check(subject));

    // release resources and conclude
    delete_tokenlist(subject);

    assert_zero_allocations();
    conclude_test_domain();
}

static void convert_test(void)
{
    begin_test_domain("Convert");
    TokenList subject = new_tokenlist();

    assert_error(convert("a", &subject),
            INVALID_INPUT_CHARACTER, 0);

    assert_error(convert("*", &subject),
            INVALID_TOKEN, 0);

    // test for zero length input
    assert_success(convert("", &subject));
    TokenList expected = new_tokenlist();
    assert_tokenlists_equal(expected, subject);

    // manual result verification
    assert_success(convert("(((8+9)*10)^2)-(1+2-3*4/5^6%2-1)", &subject));
    clear_tokenlist(&expected);
    tokenlist_add(&expected, create_number_token(8, 3));
    tokenlist_add(&expected, create_number_token(9, 5));
    tokenlist_add(&expected, create_operator_token(ADD, 4));
    tokenlist_add(&expected, create_number_token(10, 8));
    tokenlist_add(&expected, create_operator_token(MULT, 7));
    tokenlist_add(&expected, create_number_token(2, 12));
    tokenlist_add(&expected, create_operator_token(POW, 11));
    tokenlist_add(&expected, create_number_token(1, 16));
    tokenlist_add(&expected, create_number_token(2, 18));
    tokenlist_add(&expected, create_operator_token(ADD, 17));
    tokenlist_add(&expected, create_number_token(3, 20));
    tokenlist_add(&expected, create_number_token(4, 22));
    tokenlist_add(&expected, create_operator_token(MULT, 21));
    tokenlist_add(&expected, create_number_token(5, 24));
    tokenlist_add(&expected, create_number_token(6, 26));
    tokenlist_add(&expected, create_operator_token(POW, 25));
    tokenlist_add(&expected, create_operator_token(DIV, 23));
    tokenlist_add(&expected, create_number_token(2, 28));
    tokenlist_add(&expected, create_operator_token(MOD, 27));
    tokenlist_add(&expected, create_operator_token(SUB, 19));
    tokenlist_add(&expected, create_number_token(1, 30));
    tokenlist_add(&expected, create_operator_token(SUB, 29));
    tokenlist_add(&expected, create_operator_token(SUB, 14));
    assert_tokenlists_equal(expected, subject);

    assert_success(convert("-5", &subject));
    clear_tokenlist(&expected);
    tokenlist_add(&expected, create_number_token(-5, 1));
    assert_tokenlists_equal(expected, subject);

    assert_success(convert(" - (5)", &subject));
    clear_tokenlist(&expected);
    tokenlist_add(&expected, create_number_token(5, 4));
    tokenlist_add(&expected, create_operator_token(NEG, 1));
    assert_tokenlists_equal(expected, subject);

    assert_success(convert("sind 90", &subject));
    clear_tokenlist(&expected);
    tokenlist_add(&expected, create_number_token(90, 5));
    tokenlist_add(&expected, create_operator_token(SIND, 0));
    assert_tokenlists_equal(expected, subject);

    assert_success(convert("-cos -90", &subject));
    clear_tokenlist(&expected);
    tokenlist_add(&expected, create_number_token(-90, 6));
    tokenlist_add(&expected, create_operator_token(COS, 1));
    tokenlist_add(&expected, create_operator_token(NEG, 0));
    assert_tokenlists_equal(expected, subject);

    assert_success(convert("sin cos tan 90", &subject));
    clear_tokenlist(&expected);
    tokenlist_add(&expected, create_number_token(90, 12));
    tokenlist_add(&expected, create_operator_token(TAN, 8));
    tokenlist_add(&expected, create_operator_token(COS, 4));
    tokenlist_add(&expected, create_operator_token(SIN, 0));
    assert_tokenlists_equal(expected, subject);

    assert_success(convert("-sin cos -tan -90", &subject));
    clear_tokenlist(&expected);
    tokenlist_add(&expected, create_number_token(-90, 15));
    tokenlist_add(&expected, create_operator_token(TAN, 10));
    tokenlist_add(&expected, create_operator_token(NEG, 9));
    tokenlist_add(&expected, create_operator_token(COS, 5));
    tokenlist_add(&expected, create_operator_token(SIN, 1));
    tokenlist_add(&expected, create_operator_token(NEG, 0));
    assert_tokenlists_equal(expected, subject);

    // release resources and conclude
    delete_tokenlist(expected);
    delete_tokenlist(subject);

    assert_zero_allocations();
    conclude_test_domain();
}

static void parse_test(void)
{
    begin_test_domain("Parse");
    Token subject = create_empty_token();

    assert_error(parse("a", &subject),
            INVALID_INPUT_CHARACTER, 0);

    assert_error(parse("*", &subject),
            INVALID_TOKEN, 0);

    assert_error(parse("1 / 0", &subject),
            ZERO_DIVISON, 2);

    assert_error(parse("1.0 / 0", &subject),
            ZERO_DIVISON, 4);

    assert_error(parse("-1 ^ 0.25", &subject),
            NEGATIVE_FRACTIONAL_EXPONENT, 3);

    assert_error(parse("0 ^ -5", &subject),
            ZERO_NEGATIVE_EXPONENT, 2);

    assert_error(parse("tan(pi/2)", &subject),
            TANGENT_UNDEFINED, 0);

    assert_error(parse("tan(5*pi/2)", &subject),
            TANGENT_UNDEFINED, 0);

    assert_error(parse("tand 90", &subject),
            TANGENT_UNDEFINED, 0);

    assert_error(parse("tand(90 + 180 * 3)", &subject),
            TANGENT_UNDEFINED, 0);

    assert_error(parse("asin 2", &subject),
            ARCUS_OUT_OF_RANGE, 0);

    assert_error(parse("acos -2", &subject),
            ARCUS_OUT_OF_RANGE, 0);

    assert_error(parse("asind -2", &subject),
            ARCUS_OUT_OF_RANGE, 0);

    assert_error(parse("acosd 2", &subject),
            ARCUS_OUT_OF_RANGE, 0);

    assert_error(parse("fac 2.5", &subject),
            FAC_INPUT_NOT_INT, 0);

    assert_error(parse("ln 0", &subject),
            LOG_OUT_OF_RANGE, 0);

    assert_error(parse("log -1", &subject),
            LOG_OUT_OF_RANGE, 0);

    // test for zero length input
    assert_parse_result("", 0);

    assert_parse_result("10/5", 2);

    assert_parse_result("10/5.0", 2.0);

    assert_parse_result("10.0 /5", 2.0);

    assert_parse_result("1/5", 0.2);

    assert_parse_result("-(2+3)", -5);

    assert_parse_result("-(2+3) + 1", -4);

    assert_parse_result("-(2^3)", -8);

    assert_parse_result("-(2^3) + 1", -7);

    assert_parse_result("2^-(1)*3", 1.5);

    assert_parse_result("-(3/6)", -0.5);

    assert_parse_result("-(3/6) + 1", 0.5);

    assert_parse_result("456-41-675*2^3-15", -5000);

    assert_parse_result("(456-41-675)*2^3-15", -2095);

    assert_parse_result("3+4*50/5^2%5-1", 5);

    assert_parse_result("(1+-4/2.5)*16-(7%2)^3/5", -9.8);

    assert_parse_result("((1+-4)/2.5)*16-(7%2)^3/5", -19.4);

    assert_parse_result("2^4*(10%4+17.5-5)/2.5", 92.8);

    // test that numbers that don't it into a long are not converted
    assert_parse_result("2 ^ 128",
            340282366920938463463374607431768211456.0);

    assert_parse_result("(2 ^ 128) / 2",
            170141183460469231731687303715884105728.0);

    assert_parse_result("sin pi", 0);
    assert_parse_result("cos (pi/2)", 0);
    assert_parse_result("tan (pi/4)", 1);
    assert_parse_result("asin 1", 1.570796);
    assert_parse_result("acos 0", 1.570796);
    assert_parse_result("atan pi", 1.262627);
    assert_parse_result("sind 90", 1);
    assert_parse_result("cosd 90", 0);
    assert_parse_result("tand 45", 1);
    assert_parse_result("asind 1", 90);
    assert_parse_result("acosd 1", 0);
    assert_parse_result("atand 1", 45);
    assert_parse_result("ln 2", 0.693147);
    assert_parse_result("log 100", 2);

    assert_parse_result("sind90^2", 1);
    assert_parse_result("sin cos 0", 0.841471);

    assert_parse_result("abs -5.25", 5.25);
    assert_parse_result("abs 5.25", 5.25);

    assert_parse_result("fac 5", 120);
    assert_parse_result("fac -5", -120);

    // release resources and conclude
    assert_zero_allocations();
    conclude_test_domain();
}

int main()
{
    lexer_test();
    syntax_check_test();
    convert_test();
    parse_test();
}
