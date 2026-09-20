#define BOOST_TEST_MODULE OrderQueueTest
#include <boost/test/unit_test.hpp>

#include <sstream>

#include "order_queue.hpp"

using namespace smirnova;

BOOST_AUTO_TEST_CASE(empty_and_size)
{
  OrderQueue queue;
  BOOST_CHECK(queue.empty());
  queue.enqueue({"Item", "111", "Ivanov", "North"});
  BOOST_CHECK(!queue.empty());
  BOOST_CHECK_EQUAL(queue.size(), 1u);
}

BOOST_AUTO_TEST_CASE(has_orders_for_phone)
{
  OrderQueue queue;
  queue.enqueue({"Item", "111", "Ivanov", "North"});
  BOOST_CHECK(queue.hasOrdersForPhone("111"));
  BOOST_CHECK(!queue.hasOrdersForPhone("999"));
}

BOOST_AUTO_TEST_CASE(active_zones_are_unique)
{
  OrderQueue queue;
  queue.enqueue({"Item1", "111", "Ivanov", "North"});
  queue.enqueue({"Item2", "222", "Petrov", "North"});
  queue.enqueue({"Item3", "333", "Sidorov", "South"});

  std::set< std::string > zones = queue.activeZones();
  BOOST_CHECK_EQUAL(zones.size(), 2u);
  BOOST_CHECK(zones.count("North") == 1);
  BOOST_CHECK(zones.count("South") == 1);
}

BOOST_AUTO_TEST_CASE(count_per_zone)
{
  OrderQueue queue;
  queue.enqueue({"Item1", "111", "Ivanov", "North"});
  queue.enqueue({"Item2", "222", "Petrov", "North"});
  queue.enqueue({"Item3", "333", "Sidorov", "South"});

  auto counts = queue.countPerZone();
  BOOST_CHECK_EQUAL(counts.find("North")->second, 2);
  BOOST_CHECK_EQUAL(counts.find("South")->second, 1);
  BOOST_CHECK(counts.find("East") == counts.end());
}

BOOST_AUTO_TEST_CASE(extract_by_zone_preserves_arrival_order_of_rest)
{
  OrderQueue queue;
  queue.enqueue({"Fridge", "111", "Ivanov", "North"});
  queue.enqueue({"Sofa", "222", "Petrov", "South"});
  queue.enqueue({"Table", "333", "Sidorov", "North"});

  std::vector< Order > north = queue.extractByZone("North");
  BOOST_REQUIRE_EQUAL(north.size(), 2u);
  BOOST_CHECK_EQUAL(north[0].item, "Fridge");
  BOOST_CHECK_EQUAL(north[1].item, "Table");

  BOOST_CHECK_EQUAL(queue.size(), 1u);
  BOOST_CHECK(queue.activeZones().count("North") == 0);
  BOOST_CHECK(queue.activeZones().count("South") == 1);
}

BOOST_AUTO_TEST_CASE(print_empty_queue)
{
  OrderQueue queue;
  std::ostringstream out;
  queue.print(out);
  BOOST_CHECK_EQUAL(out.str(), "<QUEUE IS EMPTY>\n");
}

BOOST_AUTO_TEST_CASE(print_lists_orders_in_arrival_order)
{
  OrderQueue queue;
  queue.enqueue({"Fridge", "123", "Kozlov", "North"});
  queue.enqueue({"Sofa", "456", "Ivanov", "East"});
  std::ostringstream out;
  queue.print(out);
  std::string text = out.str();
  BOOST_CHECK(text.find("1. Fridge") != std::string::npos);
  BOOST_CHECK(text.find("2. Sofa") != std::string::npos);
}
