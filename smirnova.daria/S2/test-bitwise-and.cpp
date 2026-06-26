#define BOOST_TEST_MODULE BitwiseAndTest
#include <boost/test/unit_test.hpp>

#include <stdexcept>

#include "eval.hpp"

BOOST_AUTO_TEST_CASE(simple_and)
{
  BOOST_CHECK_EQUAL(smirnova::evalExpression("3 & 5"), 1LL);
}

BOOST_AUTO_TEST_CASE(and_with_zero)
{
  BOOST_CHECK_EQUAL(smirnova::evalExpression("0 & 255"), 0LL);
}

BOOST_AUTO_TEST_CASE(and_all_bits_set)
{
  BOOST_CHECK_EQUAL(smirnova::evalExpression("255 & 170"), 170LL);
}

BOOST_AUTO_TEST_CASE(and_same_values)
{
  BOOST_CHECK_EQUAL(smirnova::evalExpression("7 & 7"), 7LL);
}

BOOST_AUTO_TEST_CASE(and_lower_precedence_than_addition)
{
  BOOST_CHECK_EQUAL(smirnova::evalExpression("3 + 5 & 7"), 0LL);
}

BOOST_AUTO_TEST_CASE(and_with_parentheses)
{
  BOOST_CHECK_EQUAL(smirnova::evalExpression("( 3 + 5 ) & 7"), 0LL);
  BOOST_CHECK_EQUAL(smirnova::evalExpression("3 & ( 5 + 7 )"), 0LL);
}

BOOST_AUTO_TEST_CASE(and_of_two_sums)
{
  BOOST_CHECK_EQUAL(smirnova::evalExpression("( 1 + 2 ) & ( 2 + 1 )"), 3LL);
}

BOOST_AUTO_TEST_CASE(and_bit_pattern)
{
  BOOST_CHECK_EQUAL(smirnova::evalExpression("12 & 10"), 8LL);
}

BOOST_AUTO_TEST_CASE(and_chained)
{
  BOOST_CHECK_EQUAL(smirnova::evalExpression("15 & 6 & 3"), 2LL);
}

BOOST_AUTO_TEST_CASE(and_lower_precedence_than_multiplication)
{
  BOOST_CHECK_EQUAL(smirnova::evalExpression("2 * 3 & 7"), 6LL);
}
