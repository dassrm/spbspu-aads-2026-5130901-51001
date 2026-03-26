#define BOOST_TEST_MODULE StackTest
#include <boost/test/unit_test.hpp>

#include <stdexcept>
#include <string>

#include "stack.hpp"

BOOST_AUTO_TEST_CASE(lifo_order)
{
  smirnova::Stack< int > s;
  s.push(1);
  s.push(2);
  s.push(3);
  BOOST_CHECK_EQUAL(s.drop(), 3);
  BOOST_CHECK_EQUAL(s.drop(), 2);
  BOOST_CHECK_EQUAL(s.drop(), 1);
}

BOOST_AUTO_TEST_CASE(top_does_not_remove)
{
  smirnova::Stack< int > s;
  s.push(42);
  BOOST_CHECK_EQUAL(s.top(), 42);
  BOOST_CHECK_EQUAL(s.size(), 1u);
  BOOST_CHECK_EQUAL(s.top(), 42);
}

BOOST_AUTO_TEST_CASE(empty_and_size)
{
  smirnova::Stack< int > s;
  BOOST_CHECK(s.empty());
  BOOST_CHECK_EQUAL(s.size(), 0u);
  s.push(1);
  BOOST_CHECK(!s.empty());
  BOOST_CHECK_EQUAL(s.size(), 1u);
  s.push(2);
  BOOST_CHECK_EQUAL(s.size(), 2u);
  s.drop();
  BOOST_CHECK_EQUAL(s.size(), 1u);
  s.drop();
  BOOST_CHECK(s.empty());
}

BOOST_AUTO_TEST_CASE(drop_empty_throws)
{
  smirnova::Stack< int > s;
  BOOST_CHECK_THROW(s.drop(), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(top_empty_throws)
{
  smirnova::Stack< int > s;
  BOOST_CHECK_THROW(s.top(), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(move_semantics)
{
  smirnova::Stack< std::string > s;
  s.push(std::string("hello"));
  s.push(std::string("world"));
  BOOST_CHECK_EQUAL(s.drop(), "world");
  BOOST_CHECK_EQUAL(s.drop(), "hello");
}
