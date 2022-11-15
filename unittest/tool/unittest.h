#ifndef UNITTEST
#define UNITTEST

// project includes
#include "parser.h"

void begin_test_domain(const char *name);
void conclude_test_domain(void);
void assert_tokenlists_equal(TokenList expected, TokenList result);
void assert_error(ResultInfo input, error_type errtype, unsigned int index);
void assert_success(ResultInfo input);
void assert_parse_result(const char *input, double expected_result);
void assert_zero_allocations(void);

#endif // UNITTEST
