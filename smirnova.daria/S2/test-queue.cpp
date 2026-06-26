#define BOOST_TEST_MODULE QueueTest
#include <boost/test/unit_test.hpp>

#include <stdexcept>
#include <string>

#include "queue.hpp"

BOOST_AUTO_TEST_CASE(fifo_order)
{
  smirnova::Queue< int > q;
  q.push(1);
  q.push(2);
  q.push(3);
  BOOST_CHECK_EQUAL(q.drop(), 1);
  BOOST_CHECK_EQUAL(q.drop(), 2);
  BOOST_CHECK_EQUAL(q.drop(), 3);
}

BOOST_AUTO_TEST_CASE(front_does_not_remove)
{
  smirnova::Queue< int > q;
  q.push(7);
  BOOST_CHECK_EQUAL(q.front(), 7);
  BOOST_CHECK_EQUAL(q.size(), 1u);
  BOOST_CHECK_EQUAL(q.front(), 7);
}

BOOST_AUTO_TEST_CASE(empty_and_size)
{
  smirnova::Queue< int > q;
  BOOST_CHECK(q.empty());
  BOOST_CHECK_EQUAL(q.size(), 0u);
  q.push(1);
  BOOST_CHECK(!q.empty());
  BOOST_CHECK_EQUAL(q.size(), 1u);
  q.push(2);
  BOOST_CHECK_EQUAL(q.size(), 2u);
  q.drop();
  BOOST_CHECK_EQUAL(q.size(), 1u);
  q.drop();
  BOOST_CHECK(q.empty());
}

BOOST_AUTO_TEST_CASE(drop_empty_throws)
{
  smirnova::Queue< int > q;
  BOOST_CHECK_THROW(q.drop(), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(front_empty_throws)
{
  smirnova::Queue< int > q;
  BOOST_CHECK_THROW(q.front(), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(copy_constructor)
{
  smirnova::Queue< int > q;
  q.push(10);
  q.push(20);
  smirnova::Queue< int > copy(q);
  BOOST_CHECK_EQUAL(copy.drop(), 10);
  BOOST_CHECK_EQUAL(copy.drop(), 20);
  BOOST_CHECK_EQUAL(q.drop(), 10);
  BOOST_CHECK_EQUAL(q.drop(), 20);
}

BOOST_AUTO_TEST_CASE(move_constructor)
{
  smirnova::Queue< int > q;
  q.push(5);
  q.push(6);
  smirnova::Queue< int > moved(std::move(q));
  BOOST_CHECK(q.empty());
  BOOST_CHECK_EQUAL(moved.drop(), 5);
  BOOST_CHECK_EQUAL(moved.drop(), 6);
}

BOOST_AUTO_TEST_CASE(push_after_drain_and_repush)
{
  smirnova::Queue< int > q;
  q.push(1);
  q.drop();
  q.push(2);
  q.push(3);
  BOOST_CHECK_EQUAL(q.drop(), 2);
  BOOST_CHECK_EQUAL(q.drop(), 3);
}
